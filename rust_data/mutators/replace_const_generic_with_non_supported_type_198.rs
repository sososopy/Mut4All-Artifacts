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

pub struct Replace_Const_Generic_With_Non_Supported_Type_198;

impl Mutator for Replace_Const_Generic_With_Non_Supported_Type_198 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Non_Supported_Type_198"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstGenericReplacer<'a> {
            new_type_ident: &'a Ident,
        }

        impl<'a> VisitMut for ConstGenericReplacer<'a> {
            fn visit_item_struct_mut(&mut self, i: &mut ItemStruct) {
                let generics = &mut i.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(self.new_type_ident.clone()),
                        });
                    }
                }
                visit_mut::visit_item_struct_mut(self, i);
            }

            fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
                let generics = &mut i.sig.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(self.new_type_ident.clone()),
                        });
                    }
                }
                visit_mut::visit_item_fn_mut(self, i);
            }
        }

        let new_type_ident = Ident::new("Dimension", Span::call_site());
        let mut replacer = ConstGenericReplacer {
            new_type_ident: &new_type_ident,
        };

        replacer.visit_file_mut(file);

        let new_type = parse_quote! {
            struct Dimension;
        };
        file.items.insert(0, Item::Struct(new_type));
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies const generic parameters in structs, traits, and functions, and replaces their types with a non-supported type. The non-supported type is declared if it is newly introduced, aiming to trigger compiler errors or crashes related to const generic parameter handling."
    }
}