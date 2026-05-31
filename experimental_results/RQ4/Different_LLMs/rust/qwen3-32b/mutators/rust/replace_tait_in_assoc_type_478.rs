use crate::mutator::Mutator;

pub struct Replace_TAIT_In_Assoc_Type_478;

impl Mutator for Replace_TAIT_In_Assoc_Type_478 {
    fn name(&self) -> &str {
        "Replace_TAIT_In_Assoc_Type_478"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut tait_types = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    tait_types.push((item_type.ident.clone(), type_impl_trait.bounds.clone()));
                }
            }
        }

        if !tait_types.is_empty() {
            let (tait_ident, _) = &tait_types[0];
            for item in &mut file.items {
                if let syn::Item::Impl(impl_item) = item {
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Type(assoc_type) = impl_item {
                            assoc_type.ty = syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = syn::punctuated::Punctuated::new();
                                        segs.push(syn::PathSegment {
                                            ident: tait_ident.clone(),
                                            arguments: syn::PathArguments::None,
                                        });
                                        segs
                                    },
                                },
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces associated types in trait implementations with opaque type aliases (TAIT). This transformation forces the compiler to resolve TAITs as trait-associated types, potentially exposing bugs in type inference, trait resolution, or monomorphization logic when opaque types are used in complex trait hierarchies."
    }
}