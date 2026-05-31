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

pub struct Wrap_Associated_Type_Projection_With_Generic_Struct_276;

impl Mutator for Wrap_Associated_Type_Projection_With_Generic_Struct_276 {
    fn name(&self) -> &str {
        "Wrap_Associated_Type_Projection_With_Generic_Struct_276"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                let mut replacements = Vec::new();
                let mut visitor = AssociatedTypeVisitor {
                    replacements: &mut replacements,
                    wrapper_counter: 0,
                };
                visitor.visit_item_impl_mut(impl_item);
                let mut new_items = Vec::new();
                for (mut target_type, wrapper_name, projection) in replacements {
                    let wrapper_name_clone = wrapper_name.clone();
                    let wrapper_struct = Item::Struct(ItemStruct {
                        attrs: Vec::new(),
                        vis: syn::Visibility::Inherited,
                        struct_token: token::Struct::default(),
                        ident: wrapper_name_clone,
                        generics: Generics {
                            lt_token: None,
                            params: Punctuated::from_iter(vec![GenericParam::Type(TypeParam {
                                attrs: Vec::new(),
                                ident: Ident::new("T", Span::call_site()),
                                colon_token: Some(token::Colon::default()),
                                bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(
                                    TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath::from(Ident::new("Sized", Span::call_site())),
                                    },
                                )]),
                                default: None,
                                eq_token: None,
                            })]),
                            gt_token: None,
                            where_clause: None,
                        },
                        fields: syn::Fields::Unnamed(syn::FieldsUnnamed {
                            paren_token: token::Paren::default(),
                            unnamed: Punctuated::from_iter(vec![syn::Field {
                                attrs: Vec::new(),
                                vis: syn::Visibility::Inherited,
                                colon_token: Some(token::Colon::default()),
                                ty: Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(Ident::new("T", Span::call_site())),
                                }),
                                ident: None,
                                mutability: syn::FieldMutability::None,
                            }]),
                        }),
                        semi_token: None,
                    });
                    new_items.push(wrapper_struct);
                    *target_type = Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![
                                syn::PathSegment {
                                    ident: wrapper_name,
                                    arguments: PathArguments::AngleBracketed(
                                        syn::AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt::default(),
                                            args: Punctuated::from_iter(vec![GenericArgument::Type(
                                                projection,
                                            )]),
                                            gt_token: token::Gt::default(),
                                        },
                                    ),
                                },
                            ]),
                        },
                    });
                }
                if !new_items.is_empty() {
                    impl_item.items.extend(new_items.into_iter().map(|item| match item {
                        Item::Struct(s) => syn::ImplItem::Verbatim(quote!(#s).into()),
                        _ => panic!("unexpected item type"),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets associated type projections within anonymous types inside trait implementations. It identifies projections like `<Self as Trait>::AssocType` within union, struct, or enum fields/variants. For each such projection, it creates a new generic wrapper struct with a `Sized` bound and replaces the projection with an instantiation of the wrapper. This adds intermediate generic layers, increasing type normalization complexity and stressing compiler trait solving and region erasure, potentially triggering ICEs."
    }
}

struct AssociatedTypeVisitor<'a> {
    replacements: &'a mut Vec<(Box<Type>, Ident, Type)>,
    wrapper_counter: usize,
}

impl<'a> VisitMut for AssociatedTypeVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(TypePath { qself: None, path }) = ty {
            if path.segments.len() == 1 {
                let segment = &path.segments[0];
                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    if args.args.len() == 1 {
                        if let GenericArgument::Type(Type::Path(TypePath { qself: Some(qself), path: inner_path })) = &args.args[0] {
                            if qself.position == 0 && qself.as_token.is_some() && inner_path.segments.len() == 1 {
                                let wrapper_name = Ident::new(&format!("Wrapper_{}", self.wrapper_counter), Span::call_site());
                                self.wrapper_counter += 1;
                                let projection = Type::Path(TypePath {
                                    qself: Some(qself.clone()),
                                    path: inner_path.clone(),
                                });
                                self.replacements.push((Box::new(ty.clone()), wrapper_name, projection));
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_fields_mut(&mut self, fields: &mut syn::Fields) {
        match fields {
            syn::Fields::Named(syn::FieldsNamed { named, .. }) => {
                for field in named {
                    self.visit_type_mut(&mut field.ty);
                }
            }
            syn::Fields::Unnamed(syn::FieldsUnnamed { unnamed, .. }) => {
                for field in unnamed {
                    self.visit_type_mut(&mut field.ty);
                }
            }
            syn::Fields::Unit => {}
        }
    }

    fn visit_variant_mut(&mut self, variant: &mut syn::Variant) {
        self.visit_fields_mut(&mut variant.fields);
    }
}