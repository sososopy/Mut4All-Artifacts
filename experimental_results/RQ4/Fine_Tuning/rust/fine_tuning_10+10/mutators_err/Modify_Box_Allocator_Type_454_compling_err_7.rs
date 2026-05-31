use crate::mutator::Mutator;
use syn::{parse_quote, Expr, ExprCall, ExprPath, GenericArgument, Item, PathArguments, Stmt, Type, TypePath};

pub struct Modify_Box_Allocator_Type_454;

impl Mutator for Modify_Box_Allocator_Type_454 {
    fn name(&self) -> &str {
        "Modify_Box_Allocator_Type_454"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut replace_box = |expr: &mut Expr| {
                    if let Expr::Call(ExprCall { func, args, .. }) = expr {
                        if let Expr::Path(ExprPath { path, .. }) = &**func {
                            if path.segments.len() == 1 && path.segments[0].ident == "Box" {
                                if let PathArguments::AngleBracketed(args) = &mut path.segments[0].arguments {
                                    if args.args.len() == 2 {
                                        args.args.pop();
                                        args.args.push(GenericArgument::Type(Type::Path(TypePath {
                                            qself: None,
                                            path: parse_quote!(std::alloc::Global),
                                        })));
                                    }
                                }
                                if args.len() == 2 {
                                    args.pop();
                                }
                            }
                        }
                    }
                };
                
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        replace_box(expr);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets Box instantiations with custom allocators, replacing the allocator type with `std::alloc::Global` and adjusting the `new_in` method to `new`. This transformation tests the compiler's handling of allocator API changes and its ability to adapt to standard allocation paths, potentially revealing issues in allocator management and interaction with the debuginfo subsystem."
    }
}