use syn::{
    parse_quote, Fields, Item, Path as SynPath, PathArguments, PathSegment, Type, TypeImplTrait,
    TypeParamBound, TraitBound, TraitBoundModifier,
};
use syn::punctuated::Punctuated;
use syn::token;
use proc_macro2::{Ident, Span};
use rand::thread_rng;
use rand::Rng;
use crate::mutator::Mutator;

pub struct Struct_With_Impl_Trait_Field_409;

impl Mutator for Struct_With_Impl_Trait_Field_409 {
    fn name(&self) -> &str {
        "Struct_With_Impl_Trait_Field_409"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let trait_name = Ident::new("Display", Span::call_site()); // Corrected trait name initialization

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut field_mutated = false;

                for field in &mut item_struct.fields {
                    if rng.gen_bool(0.5) {
                        field.ty = Type::ImplTrait(TypeImplTrait {
                            impl_token: token::Impl { span: Span::call_site() },
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(TypeParamBound::Trait(TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            segs.push(PathSegment {
                                                ident: trait_name.clone(), // Use the corrected trait name
                                                arguments: PathArguments::None,
                                            });
                                            segs
                                        },
                                    },
                                }));
                                bounds
                            },
                        });
                        field_mutated = true;
                        break;
                    }
                }

                if !field_mutated {
                    if let Fields::Named(fields_named) = &mut item_struct.fields {
                        fields_named.named.push(parse_quote! {
                            new_field: impl #trait_name
                        });
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions, modifying or introducing fields to use `impl Trait`. This transformation tests the compiler's handling of opaque types in struct fields, aiming to uncover issues in trait bounds resolution and type inference when `impl Trait` is used in non-function contexts."
    }
}