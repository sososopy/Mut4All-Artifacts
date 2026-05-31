use crate::mutator::Mutator;
use syn::parse_quote;

struct Replace_Const_Impl_Type_With_Generic_314;

impl Mutator for Replace_Const_Impl_Type_With_Generic_314 {
    fn name(&self) -> &str {
        "Replace_Const_Impl_Type_With_Generic_314"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some(syn::Impl::Defaultness::Const(_)) = impl_item.defaultness {
                    if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                        if type_path.path.segments.len() == 1 {
                            let segment = &type_path.path.segments[0];
                            if segment.arguments.is_empty() {
                                let trait_name = segment.ident.to_string();
                                for file_item in &mut file.items {
                                    if let syn::Item::Trait(trait_item) = file_item {
                                        if trait_item.ident == trait_name {
                                            if trait_item.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                                                trait_item.attrs.retain(|attr| !attr.path().is_ident("const_trait"));
                                                let new_type = parse_quote! { T };
                                                impl_item.self_ty = Box::new(new_type);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies const trait implementations for concrete types and replaces the type with an unbounded generic parameter T, while removing the #[const_trait] attribute from the trait. This creates invalid const impls on undefined types, testing the compiler's error handling and const trait resolution logic."
    }
}