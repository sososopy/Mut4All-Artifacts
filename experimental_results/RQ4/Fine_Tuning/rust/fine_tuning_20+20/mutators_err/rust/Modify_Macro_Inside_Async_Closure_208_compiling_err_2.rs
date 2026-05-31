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

pub struct Modify_Macro_Inside_Async_Closure_208;

impl Mutator for Modify_Macro_Inside_Async_Closure_208 {
    fn name(&self) -> &str {
        "Modify_Macro_Inside_Async_Closure_208"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AsyncClosureMacroVisitor {
            found: bool,
        }
        impl VisitMut for AsyncClosureMacroVisitor {
            fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
                if self.found {
                    return;
                }
                if node.asyncness.is_some() {
                    node.body.as_mut().visit_mut_with(self);
                }
            }
            fn visit_expr_macro_mut(&mut self, node: &mut ExprMacro) {
                if self.found {
                    return;
                }
                let macro_name = node.mac.path.segments.last().unwrap().ident.to_string();
                let undefined_ident = Ident::new("undefined_var", Span::call_site());
                if macro_name == "println" {
                    node.mac.tokens = quote! {
                        "{:?}", #undefined_ident
                    };
                    self.found = true;
                } else if macro_name == "print" {
                    node.mac.tokens = quote! {
                        "{:?}", #undefined_ident
                    };
                    self.found = true;
                } else if macro_name == "eprintln" {
                    node.mac.tokens = quote! {
                        "{:?}", #undefined_ident
                    };
                    self.found = true;
                } else if macro_name == "eprint" {
                    node.mac.tokens = quote! {
                        "{:?}", #undefined_ident
                    };
                    self.found = true;
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = AsyncClosureMacroVisitor { found: false };
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macros within async closures, specifically modifying common output macros like `println!`, `print!`, `eprintln!`, and `eprint!` to include an undefined variable `undefined_var` in their invocation. By altering the macro call to use an undeclared identifier, it aims to provoke macro expansion errors and stress-test the compiler's error handling for unresolved macro patterns within the context of async closures."
    }
}