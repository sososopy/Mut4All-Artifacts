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

pub struct Alter_Macro_With_Const_Generics_110;

impl Mutator for Alter_Macro_With_Const_Generics_110 {
    fn name(&self) -> &str {
        "Alter_Macro_With_Const_Generics_110"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = item_struct.generics.params.first() {
                    if let GenericParam::Const(const_param) = generics {
                        if let Some(Expr::Block(block)) = item_struct.fields.iter().find_map(|field| {
                            if let Type::Path(type_path) = &field.ty {
                                if let Some(PathArguments::AngleBracketed(args)) = type_path.path.segments.first().map(|seg| &seg.arguments) {
                                    if let Some(GenericArgument::Const(expr)) = args.args.first() {
                                        return Some(expr);
                                    }
                                }
                            }
                            None
                        }) {
                            if let Expr::Macro(expr_macro) = block.stmts.first().map(|stmt| match stmt {
                                Stmt::Expr(expr) => expr,
                                _ => return None,
                            }).unwrap() {
                                let macro_ident = &expr_macro.mac.path.segments.first().unwrap().ident;
                                let new_macro: ItemMacro = parse_quote! {
                                    macro_rules! outer_wrap {
                                        ($x:expr) => {
                                            #macro_ident!($x)
                                        };
                                    }
                                };
                                let new_expr: Expr = parse_quote! {
                                    outer_wrap!(#const_param.ident)
                                };
                                *expr_macro = ExprMacro {
                                    attrs: vec![],
                                    mac: Macro {
                                        path: parse_quote!(outer_wrap),
                                        bang_token: token::Bang { spans: [Span::call_site()] },
                                        delimiter: MacroDelimiter::Paren(Paren::default()),
                                        tokens: new_expr.into_token_stream(),
                                    },
                                };
                                file.items.push(Item::Macro(new_macro));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions using const generics with macro invocations. It introduces a new macro wrapping around the existing macro call within the const generic parameter. This transformation tests the compiler's macro expansion and const generic handling capabilities, potentially leading to unexpected behavior or internal compiler errors due to the added complexity of nested macro invocations."
    }
}