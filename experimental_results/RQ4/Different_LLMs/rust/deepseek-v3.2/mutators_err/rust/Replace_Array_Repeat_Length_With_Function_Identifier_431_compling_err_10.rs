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

pub struct Replace_Array_Repeat_Length_With_Function_Identifier_431;

impl Mutator for Replace_Array_Repeat_Length_With_Function_Identifier_431 {
    fn name(&self) -> &str {
        "Replace_Array_Repeat_Length_With_Function_Identifier_431"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut function_names = HashSet::new();
        let mut visitor = FunctionCollector {
            names: &mut function_names,
        };
        visitor.visit_file(file);

        let mut rng = thread_rng();
        let mut visitor = ArrayRepeatMutator {
            function_names: &function_names,
            rng: &mut rng,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array repeat expressions of the form [expr; length] where length is a constant expression. It replaces the length expression with a function item identifier that is in scope, such as the name of a function defined in the current module or a built-in function like main. The mutation should only be applied if the function identifier is accessible and its type is not a const fn returning a suitable integer. Specifically, target array repeat expressions that appear in contexts where the array may be involved in type inference or comparisons, such as being an operand to a binary operator like ==. For example, given a seed program containing a function named foo and an array repeat expression like [value; 42], mutate it to [value; foo]. This mimics the bug-triggering pattern where a function item is used as a const argument in an array repeat length, potentially exposing edge cases in const argument evaluation and type checking. Ensure the mutated program compiles with the min_generic_const_args feature enabled to activate the relevant compiler paths."
    }
}

struct FunctionCollector<'a> {
    names: &'a mut HashSet<String>,
}

impl<'a> Visit<'a> for FunctionCollector<'a> {
    fn visit_item_fn(&mut self, node: &'a ItemFn) {
        self.names.insert(node.sig.ident.to_string());
        syn::visit::visit_item_fn(self, node);
    }
}

struct ArrayRepeatMutator<'a, R: Rng> {
    function_names: &'a HashSet<String>,
    rng: &'a mut R,
}

impl<'a, R: Rng> VisitMut for ArrayRepeatMutator<'a, R> {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Repeat(expr_repeat) = node {
            if let Expr::Lit(_) = &*expr_repeat.len {
                if let Some(func_name) = self.function_names.iter().collect::<Vec<_>>().choose(self.rng) {
                    let ident = Ident::new(func_name, Span::call_site());
                    expr_repeat.len = Box::new(Expr::Path(ExprPath {
                        attrs: Vec::new(),
                        qself: None,
                        path: SynPath::from(ident),
                    }));
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}