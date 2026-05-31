use syn::{File, GenericParam, Item, ItemEnum, ItemStruct};

struct Remove_Lifetime_Parameters_268;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Remove_Lifetime_Parameters_268 {
    fn name(&self) -> &str {
        "Remove_Lifetime_Parameters_268"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                if enum_item.generics.params.is_empty() {
                    continue;
                }
                enum_item.generics.params.iter_mut().for_each(|param| {
                    if let GenericParam::Lifetime(_) = param {
                        *param = GenericParam::Type(syn::TypeParam {
                            attrs: Default::default(),
                            ident: syn::Ident::new("dummy", proc_macro2::Span::call_site()),
                            colon_token: None,
                            bounds: Default::default(),
                            eq_token: None,
                            default: None,
                        });
                    }
                });
            }
            if let Item::Struct(struct_item) = item {
                if struct_item.generics.params.is_empty() {
                    continue;
                }
                struct_item.generics.params.iter_mut().for_each(|param| {
                    if let GenericParam::Lifetime(_) = param {
                        *param = GenericParam::Type(syn::TypeParam {
                            attrs: Default::default(),
                            ident: syn::Ident::new("dummy", proc_macro2::Span::call_site()),
                            colon_token: None,
                            bounds: Default::default(),
                            eq_token: None,
                            default: None,
                        });
                    }
                });
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes lifetime parameters from enum and struct definitions. This transformation can help trigger bugs related to lifetime management and trait implementation, by creating situations where the compiler must handle missing or incorrect lifetime information."
    }
}