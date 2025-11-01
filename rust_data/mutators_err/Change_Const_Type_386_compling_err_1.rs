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

pub struct Change_Const_Type_386;

impl Mutator for Change_Const_Type_386 {
    fn name(&self) -> &str {
        "Change_Const_Type_386"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut custom_type_defined = false;
        let mut custom_type_name = Ident::new("Dimension", Span::call_site());

        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if type_item.ident == custom_type_name {
                    custom_type_defined = true;
                    break;
                }
            }
        }

        if !custom_type_defined {
            file.items.insert(0, parse_quote! {
                type Dimension = usize;
            });
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some((_, generics)) = &mut func.sig.generics.params.iter_mut().find(|param| {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                            if path.segments.last().unwrap().ident == "usize" {
                                return true;
                            }
                        }
                    }
                    false
                }) {
                    if let GenericParam::Const(const_param) = generics {
                        const_param.ty = parse_quote!(Dimension);
                    }

                    func.block.stmts.insert(0, parse_quote! {
                        let size_check = SIZE > 10;
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with generic constant parameters. It changes the constant's type from a primitive type like `usize` to a custom type defined within the program. The custom type is either a type alias or a newtype within the same module. The function's implementation is modified to introduce logic involving the constant parameter of the custom type, pushing the compiler's type inference and checking mechanisms."
    }
}