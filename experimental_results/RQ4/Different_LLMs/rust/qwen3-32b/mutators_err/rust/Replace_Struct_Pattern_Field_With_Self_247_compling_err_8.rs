use rand::thread_rng;
use crate::mutator::Mutator;
use rand::prelude::SliceRandom;
use syn::{Item, FnArg, Pat, Stmt, Expr, Ident};

pub struct Replace_Struct_Pattern_Field_With_Self_247;

impl Mutator for Replace_Struct_Pattern_Field_With_Self_247 {
    fn name(&self) -> &str {
        "Replace_Struct_Pattern_Field_With_Self_247"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let &mut Pat::Struct(ref mut pat_struct) = Box::as_mut(&mut pat_type.pat) {
                            let fields: Vec<_> = pat_struct.fields.iter_mut().collect();
                            if let Some(field) = fields.choose(&mut thread_rng()) {
                                if let &mut Pat::Ident(ref mut ident_pat) = Box::as_mut(&mut field.pat) {
                                    let new_ident = Ident::new("Self", ident_pat.ident.span());
                                    ident_pat.ident = new_ident;
                                }
                            }
                        }
                    }
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let &mut Pat::Struct(ref mut pat_struct) = Box::as_mut(&mut local.pat) {
                            let fields: Vec<_> = pat_struct.fields.iter_mut().collect();
                            if let Some(field) = fields.choose(&mut thread_rng()) {
                                if let &mut Pat::Ident(ref mut ident_pat) = Box::as_mut(&mut field.pat) {
                                    let new_ident = Ident::new("Self", ident_pat.ident.span());
                                    ident_pat.ident = new_ident;
                                }
                            }
                        }
                    }
                    if let Stmt::Expr(expr, _) = stmt {
                        if let &mut Expr::Match(ref mut expr_match) = Box::as_mut(expr) {
                            for arm in &mut expr_match.arms {
                                if let &mut Pat::Struct(ref mut pat_struct) = Box::as_mut(&mut arm.pat) {
                                    let fields: Vec<_> = pat_struct.fields.iter_mut().collect();
                                    if let Some(field) = fields.choose(&mut thread_rng()) {
                                        if let &mut Pat::Ident(ref mut ident_pat) = Box::as_mut(&mut field.pat) {
                                            let new_ident = Ident::new("Self", ident_pat.ident.span());
                                            ident_pat.ident = new_ident;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a valid struct pattern field binding with the keyword 'Self' in deconstruction contexts. This transformation introduces a keyword where an identifier is expected, testing the compiler's pattern resolution logic and error handling for invalid identifiers in struct patterns. It applies to let bindings, match arms, and function parameters, ensuring syntactic validity by preserving modifiers like 'ref' or 'mut' while substituting the field name."
    }
}