use syn::parse_quote;
use syn::Item;
use syn::Type;
use syn::punctuated::Punctuated;
use syn::token::Comma;
use crate::mutator::Mutator;

pub struct Modify_Enum_Variant_With_PhantomData_322;

impl Mutator for Modify_Enum_Variant_With_PhantomData_322 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_With_PhantomData_322"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                for variant in &mut item_enum.variants {
                    if let syn::Fields::Unnamed(fields) = &mut variant.fields {
                        if !fields.unnamed.is_empty() {
                            let phantom_type: Type = parse_quote!(std::marker::PhantomData<i32>);
                            fields.unnamed = Punctuated::<syn::Field, Comma>::from_iter(vec![parse_quote!(#phantom_type)].into_iter().map(|ty| syn::Field {
                                attrs: Vec::new(),
                                vis: syn::Visibility::Inherited,
                                ident: None,
                                colon_token: None,
                                ty,
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets enum variants in Rust code. Specifically, it modifies a variant with a payload to use PhantomData instead. This involves replacing the existing payload of a tuple-like variant with PhantomData<i32>. This change is designed to explore potential layout issues and inconsistencies in the Rust compiler."
    }
}