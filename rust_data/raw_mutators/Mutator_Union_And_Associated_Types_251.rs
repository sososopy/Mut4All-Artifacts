use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Mutator_Union_And_Associated_Types_251;

impl Mutator for Mutator_Union_And_Associated_Types_251 {
    fn name(&self) -> &str {
        "Mutator_Union_And_Associated_Types_251"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct UnionVisitor<'ast> {
            struct_name: Option<Ident>,
            trait_bound: Option<TraitBound>,
            new_struct: Option<ItemStruct>,
        }
        
        impl<'ast> VisitMut for UnionVisitor<'ast> {
            fn visit_item_union_mut(&mut self, i: &mut ItemUnion) {
                if let Some(field) = i.fields.named.iter_mut().find(|field| {
                    if let Type::Path(TypePath { path, .. }) = &field.ty {
                        path.segments.iter().any(|seg| {
                            if let PathArguments::AngleBracketed(args) = &seg.arguments {
                                args.args.iter().any(|arg| {
                                    if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                        type_path.path.segments.iter().any(|seg| {
                                            if let PathArguments::AngleBracketed(args) = &seg.arguments {
                                                args.args.iter().any(|arg| {
                                                    if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                                        if let Some(trait_bound) = &self.trait_bound {
                                                            return type_path.path.segments.iter().any(|seg| {
                                                                seg.ident == trait_bound.path.segments.last().unwrap().ident
                                                            });
                                                        }
                                                    }
                                                    false
                                                })
                                            } else {
                                                false
                                            }
                                        })
                                    } else {
                                        false
                                    }
                                })
                            } else {
                                false
                            }
                        })
                    } else {
                        false
                    }
                }) {
                    if let Some(new_struct) = &self.new_struct {
                        field.ty = Type::Path(TypePath {
                            qself: None,
                            path: parse_quote!(#new_struct),
                        });
                    }
                }
            }
        }

        let mut visitor = UnionVisitor {
            struct_name: None,
            trait_bound: None,
            new_struct: None,
        };

        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                for item in &trait_item.items {
                    if let TraitItem::Type(type_item) = item {
                        visitor.trait_bound = Some(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: type_item.bounds.iter().filter_map(|bound| {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    Some(trait_bound.clone())
                                } else {
                                    None
                                }
                            }).next().unwrap().path.clone(),
                        });
                    }
                }
            }
        }

        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                visitor.struct_name = Some(struct_item.ident.clone());
                visitor.new_struct = Some(struct_item.clone());
            }
        }

        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets unions with fields involving associated types. It introduces a new struct that mimics the associated type's constraints and replaces a union field with this new struct."
    }
}