//source file
#include "../include/modify_destructor_declaration_with_constexpr_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
        //Filter nodes in header files
        if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(DT->getLocation()))
            return;
        
        if (DT->isUserProvided() && DT->isInlineSpecified()) {
            //Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), DT->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            if (declaration.find("constexpr") == std::string::npos) {
                declaration.insert(declaration.find("~"), "constexpr ");
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDestructorDecl(isInline()).bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}