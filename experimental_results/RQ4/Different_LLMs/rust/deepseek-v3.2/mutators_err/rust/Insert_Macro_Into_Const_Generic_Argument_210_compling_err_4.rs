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

pub struct Insert_Macro_Into_Const_Generic_Argument_210;

impl Mutator for Insert_Macro_Into_Const_Generic_Argument_210 {
    fn name(&self) -> &str {
        "Insert_Macro_Into_Const_Generic_Argument_210"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let macro_names = vec!["thread_local", "lazy_static", "vec", "println", "eprintln", "assert", "assert_eq", "assert_ne", "cfg", "env", "concat", "stringify", "include", "include_str", "include_bytes", "format", "format_args", "panic", "unreachable", "todo"];
        
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut has_const_param = false;
                for param in &item_impl.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_param = true;
                        break;
                    }
                }
                
                if has_const_param {
                    let mut new_params = Punctuated::<GenericParam, Comma>::new();
                    for param in item_impl.generics.params.iter() {
                        if let GenericParam::Const(const_param) = param {
                            let macro_name = macro_names.choose(&mut rng).unwrap();
                            let macro_invocation: Expr = parse_quote! {
                                { #macro_name! { static DUMMY: u8 = 0; } 0 }
                            };
                            let new_default: Expr = parse_quote! {
                                { #macro_invocation }
                            };
                            let new_const_param: GenericParam = parse_quote! {
                                const #macro_name: u8 = #new_default
                            };
                            new_params.push(new_const_param);
                        } else {
                            new_params.push(param.clone());
                        }
                    }
                    item_impl.generics.params = new_params;
                } else {
                    let macro_name = macro_names.choose(&mut rng).unwrap();
                    let macro_invocation: Expr = parse_2quote! {
                        { #macro_name! { static DUMMY: u8 = 0; } 0 }
                    };
                    let new_default: Expr = parse_quote! {
                        { #macro_invocation }
                    };
                    let new_const_param: GenericParam = parse_quote! {
                        const #macro_name: u8 = #new_default
                    };
                    item_impl.generics.params.push(new_const_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks, inserting macro invocations into const generic arguments. If const generic parameters exist, it replaces their default values with macro invocations wrapped in const blocks. If no const generics exist, it adds a new const generic parameter with a macro invocation default. This transformation creates complex const evaluation contexts with nested macro expansions, potentially exposing ICEs in the compiler's HIR lowering or const evaluation phases."
    }
}