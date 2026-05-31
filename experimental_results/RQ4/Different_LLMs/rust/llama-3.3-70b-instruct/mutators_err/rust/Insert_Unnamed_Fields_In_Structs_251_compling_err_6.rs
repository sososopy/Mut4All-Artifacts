use syn::{parse_quote, Item, Visibility, Fields, Field, punctuated::Punctuated, token::Paren};
use crate::mutator::Mutator;
use syn::spanned::Spanned;

struct Insert_Unnamed_Fields_In_Structs_251;

impl Mutator for Insert_Unnamed_Fields_In_Structs_251 {
    fn name(&self) -> &str {
        "Insert_Unnamed_Fields_In_Structs_251"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                let mut new_fields = Punctuated::new();
                for field in &struct_item.fields {
                    new_fields.push(field.clone());
                }
                new_fields.push(Field {
                    attrs: vec![],
                    vis: Visibility::Inherited,
                    ident: None,
                    colon_token: None,
                    ty: parse_quote!(i32),
                    mutability: syn::FieldMutability::NotMut,
                });
                struct_item.fields = Fields::Unnamed(syn::FieldsUnnamed { 
                    paren_token: Paren { span: struct_item.fields.span() },
                    unnamed: new_fields,
                });
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an unnamed field into structs. This transformation aims to test the handling of unnamed fields in structs, especially when combined with other features like -Zunpretty=expanded,hygiene, which can affect the compiler's behavior."
    }
}