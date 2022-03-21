package wit.vega2.lic.gen;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.security.AlgorithmParameters;
import java.security.MessageDigest;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.SecretKeySpec;
import javax.xml.bind.DatatypeConverter;

public class Security {
    
    private List<String> dataList;
    private String encryptedText;

    public Security(List<String> dataList) {
        this.dataList = dataList;
    }
    
    public Security(String encryptedText) {
        this.encryptedText = encryptedText;
    }

    public List<String> getDataList() {
        return dataList;
    }

    public String getEncryptedText() {
        return encryptedText;
    }
    
    public void encrypt(String password) throws Exception {
        InputStream input = new ByteArrayInputStream(serialize());
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        AES.encrypt(256, password.toCharArray(), input, output);
        encryptedText = DatatypeConverter.printBase64Binary(output.toByteArray());
    }

    public void decrypt(String password) throws Exception {
        byte[] encryptedTextBytes = DatatypeConverter.parseBase64Binary(encryptedText);
        InputStream input = new ByteArrayInputStream(encryptedTextBytes);
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        AES.decrypt(password.toCharArray(), input, output);
        deserizalize(output.toByteArray());
    }
    
    private byte[] serialize() throws LicException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream w = new DataOutputStream(baos);
        Iterator<String> iterator = dataList.iterator();
        while (iterator.hasNext()) {
            serizalizeString(w, iterator.next());
        }
        return baos.toByteArray();
    }
    
    private void deserizalize(byte[] data) throws LicException {
        dataList = new ArrayList();
        ByteArrayInputStream bais = new ByteArrayInputStream(data);
        DataInputStream r = new DataInputStream(bais);
        Iterator<String> iterator = dataList.iterator();
        String item = null;
        do {
            item = deserizalizeString(r);
            if(item != null) {
                dataList.add(item);
            }
        }
        while(item != null);
    }
            
    public static String sha256(String base) {
        try{
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] hash = digest.digest(base.getBytes("UTF-8"));
            StringBuffer hexString = new StringBuffer();
            for (int i = 0; i < hash.length; i++) {
                String hex = Integer.toHexString(0xff & hash[i]);
                if(hex.length() == 1) hexString.append('0');
                hexString.append(hex);
            }
            return hexString.toString();
        } catch(Exception ex) {
           throw new RuntimeException(ex);
        }
    }
    
    private void serizalizeString(DataOutputStream w, String s) throws LicException {
        try {
            if (s != null && !s.isEmpty()) {
                byte[] bb = s.getBytes("UTF-8");
                w.writeInt(bb.length);
                w.write(bb);
            } else {
                w.writeInt(0);
            }
        } catch (UnsupportedEncodingException ex) {
            throw new LicException(ex);
        } catch (IOException ex) {
            throw new LicException(ex);
        }
    }

    private String deserizalizeString(DataInputStream r) throws LicException {
        try {
            int len = r.readInt();
            if (len > 0) {
                byte[] bb = new byte[len];
                if (r.read(bb) == len) {
                    return new String(bb, "UTF-8");
                }
            } else {
                return "";
            }
        } catch (IOException ex) {
            return null;
        }
        return null;
    }
}
