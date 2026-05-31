//source file
#include "../include/introduce_ambiguous_member_193.h"

// ========================================================================================================
#define MUT193_OUTPUT 1

void MutatorFrontendAction_193::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        
        if (!CD->isCompleteDefinition() || processedClasses.count(CD->getNameAsString()))
            return;

        processedClasses.insert(CD->getNameAsString());

        for (auto method : CD->methods()) {
            if (method->hasBody() && !method->isOverloadedOperator()) {
                std::string newMethodDecl = "double " + method->getNameAsString() + "(double y) { return y; }";
                std::string mutation = "/*mut193*/" + newMethodDecl + "\n";
                Rewrite.InsertTextAfterToken(CD->getEndLoc().getLocWithOffset(-1), mutation);
                break;
            }
        }
    }
}

void MutatorFrontendAction_193::MutatorASTConsumer_193::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}