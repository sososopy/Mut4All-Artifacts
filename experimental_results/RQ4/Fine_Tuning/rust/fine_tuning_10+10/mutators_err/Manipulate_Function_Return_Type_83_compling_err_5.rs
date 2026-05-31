use syn::{parse_quote, Item, ReturnType, Type, FnArg, Expr, Stmt};
use crate::mutator::Mutator;

pub struct Manipulate_Function_Return_Type_83;

impl Mutator for Manipulate_Function_Return_Type_83 {
    fn name(&self) -> &str {
        "Manipulate_Function_Return_Type_83"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = **return_type {
                        let fn_name = &item_fn.sig.ident;
                        let fn_params: Vec<_> = item_fn.sig.inputs.iter().map(|arg| match arg {
                            FnArg::Typed(pat_type) => &pat_type.pat,
                            _ => panic!("Unexpected argument type"),
                        }).collect();
                        
                        let recursive_call: Expr = parse_quote! {
                            #fn_name(#(#fn_params),*)
                        };

                        let new_stmt: Stmt = parse_quote! {
                            #recursive_call;
                        };

                        item_fn.block.stmts.push(new_stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with `impl Trait` in their return type. It modifies the function body to include a recursive call to itself, ensuring the call matches the original function signature. This stresses the compiler's trait resolution logic in recursive contexts, potentially exposing issues in trait projection and opaque type handling."
    }
}