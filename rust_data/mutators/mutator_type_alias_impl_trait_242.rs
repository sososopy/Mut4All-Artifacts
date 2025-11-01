use crate::mutator::Mutator;
use syn::{parse_quote, File, GenericParam, Ident, Item, Type, TypeParamBound};
use syn::punctuated::Punctuated;
use proc_macro2::Span;

pub struct Mutator_Type_Alias_Impl_Trait_242;

impl Mutator for Mutator_Type_Alias_Impl_Trait_242 {
    fn name(&self) -> &str {
        "Mutator_Type_Alias_Impl_Trait_242"
    }
    fn mutate(&self, file: &mut File) {
        let mut new_trait_added = false;
        let mut alias_name = None;
        let mut trait_name = None;

        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(type_impl_trait) = *type_item.ty.clone() {
                    if let Some(first_bound) = type_impl_trait.bounds.first() {
                        if let TypeParamBound::Trait(trait_bound) = first_bound {
                            alias_name = Some(type_item.ident.clone());
                            trait_name = Some(trait_bound.path.clone());
                            break;
                        }
                    }
                }
            }
        }

        if let (Some(alias), Some(trait_path)) = (alias_name, trait_name) {
            for item in &mut file.items {
                if let Item::Trait(trait_item) = item {
                    if trait_item.ident == alias {
                        continue;
                    }
                }
            }

            let new_trait_ident = Ident::new("NewTrait", Span::call_site());
            let new_trait: Item = parse_quote! {
                trait #new_trait_ident {}
            };
            file.items.push(new_trait);

            let impl_new_trait: Item = parse_quote! {
                impl #new_trait_ident for #alias {}
            };
            file.items.push(impl_new_trait);

            for item in &mut file.items {
                if let Item::Impl(impl_item) = item {
                    if let Some((_, path, _)) = &impl_item.trait_ {
                        if path == &trait_path {
                            let new_bound: TypeParamBound = parse_quote!(#new_trait_ident);
                            impl_item.generics.params.push(GenericParam::Type(parse_quote!(T: #new_trait_ident)));
                            impl_item.trait_ = Some((None, path.clone(), syn::token::For { span: Span::call_site() }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies type aliases defined with `impl Trait` and introduces a new trait, `NewTrait`. It implements `NewTrait` for the type alias and modifies existing trait implementations to include a bound on `NewTrait`, creating potential conflicts in trait resolution."
    }
}