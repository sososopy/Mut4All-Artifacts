//source file
#include "../include/Mutator_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
        //Filter nodes in header files
        if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
            return;
        
        // Check if the destructor is already constexpr
        if (DT->isConstexpr())
            return;

        // Ensure the class has at least one non-trivial member
        const auto *ParentClass = DT->getParent();
        bool hasNonTrivialMember = false;
        for (const auto *Field : ParentClass->fields()) {
            if (!Field->getType().isTriviallyCopyableType(*Result.Context)) {
                hasNonTrivialMember = true;
                break;
            }
        }

        if (!hasNonTrivialMember)
            return;

        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   DT->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("~");
        if (pos != std::string::npos) {
            declaration.insert(pos, "constexpr ");
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDestructorDecl().bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}