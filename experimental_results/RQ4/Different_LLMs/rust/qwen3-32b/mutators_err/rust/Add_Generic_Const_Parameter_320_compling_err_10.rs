use syn::parse_quote;
use crate::mutator::Mutator;

pub struct Add_Generic_Const_Parameter_320;

impl Mutator for Add_Generic_Const_Parameter_320 {
    fn name(&self) -> &str {
        "Add_Generic_Const_Parameter_320"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_traits = Vec::new();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if trait_item.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                    const_traits.push(trait_item.ident.clone());
                    for trait_item in &mut trait_item.items {
                        if let syn::TraitItem::Fn(method) = trait_item {
                            let generic = parse_quote!(const C: bool);
                            method.sig.generics.params.push(generic);
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if impl_item.defaultness.as_ref().map_or(false, |d| matches!(d, syn::Defaultness::Const(_))) {
                    if let Some((_, trait_path, _)) = &impl_item.trait_ {
                        if let Some(trait_ident) = trait_path.get_ident() {
                            if const_traits.iter().any(|t| t == trait_ident) {
                                for impl_item in &mut impl_item.items {
                                    if let syn::ImplItem::Fn(method) = impl_item {
                                        let generic = parse_quote!(const C: bool);
                                        method.sig.generics.params.push(generic);
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
        ""
    }
}