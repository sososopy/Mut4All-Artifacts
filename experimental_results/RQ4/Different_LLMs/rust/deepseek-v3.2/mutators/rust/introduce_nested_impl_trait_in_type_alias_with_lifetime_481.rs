use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Introduce_Nested_Impl_Trait_In_Type_Alias_With_Lifetime_481;

impl Mutator for Introduce_Nested_Impl_Trait_In_Type_Alias_With_Lifetime_481 {
    fn name(&self) -> &str {
        "Introduce_Nested_Impl_Trait_In_Type_Alias_With_Lifetime_481"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut found_type_alias = false;
        let mut existing_lifetimes = Vec::new();
        let mut existing_types = Vec::new();

        for item in &file.items {
            match item {
                Item::Type(type_item) => {
                    if let Type::ImplTrait(_) = *type_item.ty {
                        found_type_alias = true;
                    }
                }
                Item::Fn(func) => {
                    for param in &func.sig.generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            existing_lifetimes.push(lifetime_param.lifetime.clone());
                        }
                    }
                }
                Item::Struct(struct_item) => {
                    for param in &struct_item.generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            existing_lifetimes.push(lifetime_param.lifetime.clone());
                        }
                    }
                }
                Item::Impl(impl_item) => {
                    for param in &impl_item.generics.params {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            existing_lifetimes.push(lifetime_param.lifetime.clone());
                        }
                    }
                }
                _ => {}
            }
        }

        let mut visitor = TypeCollector { types: &mut existing_types };
        visitor.visit_file(file);

        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(type_impl_trait) = &*type_item.ty {
                    let mut rng = thread_rng();
                    let lifetime = if !existing_lifetimes.is_empty() {
                        existing_lifetimes.choose(&mut rng).unwrap().clone()
                    } else {
                        Lifetime::new("'a", Span::call_site())
                    };

                    let base_type = if !existing_types.is_empty() {
                        existing_types.choose(&mut rng).unwrap().clone()
                    } else {
                        Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("i32", Span::call_site())),
                        })
                    };

                    let nested_impl_trait = TypeImplTrait {
                        impl_token: token::Impl { span: Span::call_site() },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath::from(Ident::new("Trait", Span::call_site())),
                            }));
                            bounds
                        },
                    };

                    let tuple_type = Type::Tuple(syn::TypeTuple {
                        paren_token: token::Paren::default(),
                        elems: {
                            let mut elems = Punctuated::new();
                            elems.push(Type::Reference(syn::TypeReference {
                                and_token: token::And::default(),
                                lifetime: Some(lifetime.clone()),
                                mutability: None,
                                elem: Box::new(base_type),
                            }));
                            elems.push(Type::ImplTrait(nested_impl_trait));
                            elems
                        },
                    });

                    let iterator_path = SynPath {
                        leading_colon: None,
                        segments: {
                            let mut segments = Punctuated::new();
                            segments.push(PathSegment {
                                ident: Ident::new("Iterator", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: {
                                        let mut args = Punctuated::new();
                                        args.push(GenericArgument::AssocType(syn::AssocType {
                                            ident: Ident::new("Item", Span::call_site()),
                                            generics: None,
                                            eq_token: token::Eq::default(),
                                            ty: tuple_type,
                                        }));
                                        args
                                    },
                                    gt_token: token::Gt::default(),
                                }),
                            });
                            segments
                        },
                    };

                    let new_impl_trait = TypeImplTrait {
                        impl_token: token::Impl { span: Span::call_site() },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: iterator_path,
                            }));
                            bounds
                        },
                    };

                    if existing_lifetimes.is_empty() {
                        type_item.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: lifetime.clone(),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                    }

                    type_item.ty = Box::new(Type::ImplTrait(new_impl_trait));
                }
            }
        }

        if !found_type_alias {
            let mut rng = thread_rng();
            let lifetime = if !existing_lifetimes.is_empty() {
                existing_lifetimes.choose(&mut rng).unwrap().clone()
            } else {
                Lifetime::new("'a", Span::call_site())
            };

            let base_type = if !existing_types.is_empty() {
                existing_types.choose(&mut rng).unwrap().clone()
            } else {
                Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(Ident::new("i32", Span::call_site())),
                })
            };

            let nested_impl_trait = TypeImplTrait {
                impl_token: token::Impl { span: Span::call_site() },
                bounds: {
                    let mut bounds = Punctuated::new();
                    bounds.push(TypeParamBound::Trait(TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: None,
                        path: SynPath::from(Ident::new("Trait", Span::call_site())),
                    }));
                    bounds
                },
            };

            let tuple_type = Type::Tuple(syn::TypeTuple {
                paren_token: token::Paren::default(),
                elems: {
                    let mut elems = Punctuated::new();
                    elems.push(Type::Reference(syn::TypeReference {
                        and_token: token::And::default(),
                        lifetime: Some(lifetime.clone()),
                        mutability: None,
                        elem: Box::new(base_type),
                    }));
                    elems.push(Type::ImplTrait(nested_impl_trait));
                    elems
                },
            });

            let iterator_path = SynPath {
                leading_colon: None,
                segments: {
                    let mut segments = Punctuated::new();
                    segments.push(PathSegment {
                        ident: Ident::new("Iterator", Span::call_site()),
                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: {
                                let mut args = Punctuated::new();
                                args.push(GenericArgument::AssocType(syn::AssocType {
                                    ident: Ident::new("Item", Span::call_site()),
                                    generics: None,
                                    eq_token: token::Eq::default(),
                                    ty: tuple_type,
                                }));
                                args
                            },
                            gt_token: token::Gt::default(),
                        }),
                    });
                    segments
                },
            };

            let new_impl_trait = TypeImplTrait {
                impl_token: token::Impl { span: Span::call_site() },
                bounds: {
                    let mut bounds = Punctuated::new();
                    bounds.push(TypeParamBound::Trait(TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: None,
                        path: iterator_path,
                    }));
                    bounds
                },
            };

            let mut generics = Generics {
                lt_token: None,
                params: Punctuated::new(),
                gt_token: None,
                where_clause: None,
            };

            if existing_lifetimes.is_empty() {
                generics.params.push(GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
            }

            let new_type_alias = Item::Type(syn::ItemType {
                attrs: Vec::new(),
                vis: syn::Visibility::Public(syn::token::Pub::default()),
                type_token: token::Type::default(),
                ident: Ident::new("TAITAlias", Span::call_site()),
                generics,
                eq_token: token::Eq::default(),
                ty: Box::new(Type::ImplTrait(new_impl_trait)),
                semi_token: token::Semi::default(),
            });

            file.items.insert(0, new_type_alias);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets type alias impl trait (TAIT) definitions. It either modifies existing TAITs or introduces a new one at module level. The transformation creates a nested impl trait within an Iterator bound that includes a lifetime parameter. If no lifetime exists in the seed, it adds a new lifetime parameter 'a to the type alias. The nested structure replicates the exact pattern from the bug report that caused an ICE: a TAIT with nested opaque type within a generic bound containing a lifetime. This stresses the compiler's ability to name nested opaque types with lifetimes and may trigger internal compiler errors in the DefPath generation."
    }
}

struct TypeCollector<'a> {
    types: &'a mut Vec<Type>,
}

impl<'a> Visit<'a> for TypeCollector<'a> {
    fn visit_type(&mut self, ty: &'a Type) {
        match ty {
            Type::Path(type_path) if !type_path.path.segments.is_empty() => {
                let segment = &type_path.path.segments[0];
                if segment.ident != "Self" && segment.ident != "self" {
                    self.types.push(ty.clone());
                }
            }
            Type::Array(_) | Type::Slice(_) | Type::Tuple(_) | Type::Reference(_) => {
                self.types.push(ty.clone());
            }
            _ => {}
        }
        syn::visit::visit_type(self, ty);
    }
}