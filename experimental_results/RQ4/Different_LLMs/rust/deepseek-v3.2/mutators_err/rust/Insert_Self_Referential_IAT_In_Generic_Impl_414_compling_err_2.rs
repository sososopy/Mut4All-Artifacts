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

pub struct Insert_Self_Referential_IAT_In_Generic_Impl_414;

impl Mutator for Insert_Self_Referential_IAT_In_Generic_Impl_414 {
    fn name(&self) -> &str {
        "Insert_Self_Referential_IAT_In_Generic_Impl_414"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.generics.params.is_empty() {
                    continue;
                }
                let mut has_associated_type = false;
                for impl_item in &item_impl.items {
                    if let ImplItem::Type(_) = impl_item {
                        has_associated_type = true;
                        break;
                    }
                }
                if has_associated_type {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Type(type_item) = impl_item {
                            let self_path: syn::Path = parse_quote!(Self);
                            let mut segments = Punctuated::new();
                            segments.push(PathSegment {
                                ident: self_path.segments.first().unwrap().ident.clone(),
                                arguments: PathArguments::None,
                            });
                            segments.push(PathSegment {
                                ident: type_item.ident.clone(),
                                arguments: PathArguments::None,
                            });
                            type_item.ty = Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            });
                            return;
                        }
                    }
                } else {
                    let new_assoc_name = Ident::new("CycleAssoc", Span::call_site());
                    let self_path: syn::Path = parse_quote!(Self);
                    let mut segments = Punctuated::new();
                    segments.push(PathSegment {
                        ident: self_path.segments.first().unwrap().ident.clone(),
                        arguments: PathArguments::None,
                    });
                    segments.push(PathSegment {
                        ident: new_assoc_name.clone(),
                        arguments: PathArguments::None,
                    });
                    let new_assoc = ImplItem::Type(ImplItemType {
                        attrs: Vec::new(),
                        vis: syn::Visibility::Inherited,
                        defaultness: None,
                        type_token: token::Type::default(),
                        ident: new_assoc_name,
                        generics: Generics::default(),
                        eq_token: token::Eq::default(),
                        ty: Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments,
                            },
                        }),
                        semi_token: token::Semi::default(),
                    });
                    item_impl.items.push(new_assoc);
                    return;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic impl blocks to introduce self-referential inherent associated types. For impl blocks with existing associated types, it replaces their definitions with Self::Assoc, creating immediate cycles. For those without, it adds a new associated type CycleAssoc defined as Self::CycleAssoc. This transformation aims to trigger compiler ICEs related to query cycles in incomplete inherent_associated_types feature by exploiting type resolution dependencies in generic contexts."
    }
}