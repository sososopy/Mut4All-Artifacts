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

pub struct Modify_Closure_Parameter_List_To_Include_Const_Generics_115;

impl Mutator for Modify_Closure_Parameter_List_To_Include_Const_Generics_115 {
    fn name(&self) -> &str {
        "Modify_Closure_Parameter_List_To_Include_Const_Generics_115"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureFinder {
            closures: Vec<ExprClosure>,
        }
        impl<'ast> Visit<'ast> for ClosureFinder {
            fn visit_expr_closure(&mut self, node: &'ast ExprClosure) {
                self.closures.push(node.clone());
            }
        }
        let mut finder = ClosureFinder { closures: vec![] };
        finder.visit_file(file);
        if finder.closures.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let closure = finder.closures.choose(&mut rng).unwrap();
        let closure_span = closure.span();
        let mut new_closure = closure.clone();
        new_closure.or1_token = token::Or {
            spans: [Span::call_site(), Span::call_site()],
        };
        new_closure.lifetimes = Some(parse_quote!(const N: i32));
        let new_closure_tokens = new_closure.to_token_stream();
        let mut file_content = file.to_token_stream().to_string();
        let closure_start = closure_span.start();
        let closure_end = closure_span.end();
        let closure_range = closure_start..closure_end;
        file_content.replace_range(closure_range, &new_closure_tokens.to_string());
        let new_file = syn::parse_file(&file_content).expect("Failed to parse modified file");
        *file = new_file;
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closures by modifying their parameter list to include a const generic, specifically a `for<const N: i32>` clause. It leverages the known issue of compiler instability when const generics are introduced in closure parameters, aiming to provoke similar ICEs by transforming existing closures in the seed program. This approach maximizes the mutator's impact by focusing on a high-risk language feature in a context known to trigger compiler crashes."
    }
}