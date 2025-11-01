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

pub struct Modify_Closure_Parameter_List_With_Const_Generics_272;

impl Mutator for Modify_Closure_Parameter_List_With_Const_Generics_272 {
    fn name(&self) -> &str {
        "Modify_Closure_Parameter_List_With_Const_Generics_272"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor;

        impl VisitMut for ClosureVisitor {
            fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
                // Check if the closure has no const generic parameter
                if i.constness.is_none() {
                    // Add a const generic parameter to the closure
                    let const_generic: GenericParam = parse_quote!(const N: i32);
                    let mut generics: Punctuated<GenericParam, token::Comma> = Punctuated::new();
                    generics.push(const_generic);

                    // Modify the closure to include the const generic
                    i.constness = Some(syn::token::Const { span: Span::call_site() });
                    i.inputs.insert(0, parse_quote!(for<const N: i32>));
                }

                // Continue visiting the rest of the closure
                visit_mut::visit_expr_closure_mut(self, i);
            }
        }

        // Apply the mutation to the file
        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures by adding a const generic parameter to their parameter list. This involves identifying closures in the code and modifying their parameter list to include a const generic, such as `for<const N: i32>`. The const generic is not used within the closure body, as the goal is to reproduce scenarios that may lead to compiler instability, as observed in certain bug reports."
    }
}