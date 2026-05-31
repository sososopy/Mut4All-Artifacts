use syn::parse_quote;
use crate::mutator::Mutator;
use syn::punctuated::Punctuated;
use proc_macro2::Ident;
use proc_macro2::Span;

pub struct RpititAssociatedTypeAnonymity_191;

impl Mutator for RpititAssociatedTypeAnonymity_191 {
    fn name(&self) -> &str {
        "RpititAssociatedTypeAnonymity_191"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if let syn::ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let syn::Type::ImplTrait(impl_trait) = return_type.as_mut() {
                                let mut lifetime_b = syn::LifetimeParam::new(syn::Lifetime::new("'b", Span::call_site()));
                                method.sig.generics.params.push(syn::GenericParam::Lifetime(lifetime_b));
                                
                                let mut lifetime_a = syn::LifetimeParam::new(syn::Lifetime::new("'a", Span::call_site()));
                                lifetime_a.colon_token = Some(syn::token::Colon::default());
                                let mut bounds = Punctuated::new();
                                bounds.push(syn::Lifetime::new("'b", Span::call_site()));
                                lifetime_a.bounds = bounds;
                                method.sig.generics.params.push(syn::GenericParam::Lifetime(lifetime_a));
                                
                                let mut bounds = Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: syn::Path::from(Ident::new("ToString", Span::call_site())),
                                }));
                                bounds.push(syn::TypeParamBound::Lifetime(syn::Lifetime::new(
                                    "'b",
                                    Span::call_site(),
                                )));
                                impl_trait.bounds = bounds;
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