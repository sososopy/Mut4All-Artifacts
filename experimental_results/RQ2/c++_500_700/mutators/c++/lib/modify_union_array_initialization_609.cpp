//source file
#include "../include/modify_union_array_initialization_609.h"

// ========================================================================================================
#define MUT609_OUTPUT 1

void MutatorFrontendAction_609::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::VarDecl>("unionVar")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UD->getLocation()))
            return;
        
        if (const clang::InitListExpr *initList = llvm::dyn_cast<clang::InitListExpr>(UD->getInit())) {
            if (initList->getNumInits() > 0) {
                auto firstInit = initList->getInit(0);
                if (firstInit->getType()->isArrayType()) {
                    std::string newInit = "{3.14}";
                    Rewrite.ReplaceText(initList->getSourceRange(), newInit);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_609::MutatorASTConsumer_609::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(recordDecl(isUnion())), hasInitializer(initListExpr())).bind("unionVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}