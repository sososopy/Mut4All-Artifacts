//source file
#include "../include/Add_Concept_Constraint_To_Lambda_Parameters_138.h"

// ========================================================================================================
#define MUT138_OUTPUT 1

void MutatorFrontendAction_138::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        // Check if the lambda is in the main file
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        // Get the closure type
        const CXXRecordDecl *Closure = LE->getLambdaClass();
        if (!Closure)
            return;

        // Find the operator() method
        for (const auto *Method : Closure->methods()) {
            if (Method->getNameAsString() == "operator()") {
                for (const auto *Param : Method->parameters()) {
                    if (Param->getType()->getAs<AutoType>()) {
                        // Get the source range of the type
                        const TypeLoc TypeLoc = Param->getTypeSourceInfo()->getTypeLoc();
                        SourceRange TypeRange = TypeLoc.getSourceRange();
                        // Replace 'auto' with 'std::input_iterator auto'
                        Rewrite.ReplaceText(TypeRange, "std::input_iterator auto");
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_138::MutatorASTConsumer_138::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(lambdaExpr())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}