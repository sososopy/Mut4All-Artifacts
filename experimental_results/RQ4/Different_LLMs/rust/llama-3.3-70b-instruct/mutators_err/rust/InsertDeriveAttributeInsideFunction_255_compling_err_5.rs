use syn::{parse_quote, ItemStruct, Stmt, Ident, Visibility, Generics, Fields, Attribute};
use proc_macro2::{Span};
use rand::thread_rng;
use rand::Rng;
use rand::prelude::SliceRandom;
use crate::mutator::Mutator;

struct InsertDeriveAttributeInsideFunction_255;

impl Mutator for InsertDeriveAttributeInsideFunction_255 {
    fn name(&self) -> &str {
        "InsertDeriveAttributeInsideFunction_255"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let traits = ["Serialize", "Deserialize", "Debug", "Clone", "PartialEq"];
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let trait_name = traits.choose(&mut rng).unwrap();
                let derive_attr: Attribute = parse_quote!(#[derive(#trait_name)]);
                let struct_name = Ident::new(&format!("Struct_{}", rng.r#gen::<u32>()), Span::call_site());
                let struct_def = parse_quote! {
                    #derive_attr
                    struct #struct_name;
                };
                let stmt = Stmt::Item(syn::Item::Struct(ItemStruct {
                    attrs: vec![],
                    vis: Visibility::Inherited,
                    ident: struct_name,
                    generics: Generics::default(),
                    fields: Fields::Unit,
                    struct_token: syn::token::Struct {
                        span: Span::call_site(),
                    },
                    semi_token: None,
                }));
                func.block.stmts.insert(0, stmt);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a new derive attribute for a randomly chosen trait inside an existing function body. This transformation creates a situation where the compiler has to handle nested derive attributes, which could trigger bugs like the stack overflow in the original issue."
    }
}