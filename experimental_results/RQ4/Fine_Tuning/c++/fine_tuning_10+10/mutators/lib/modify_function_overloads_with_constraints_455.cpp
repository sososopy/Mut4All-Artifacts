//source file
#include "../include/modify_function_overloads_with_constraints_455.h"

// ========================================================================================================
#define MUT455_OUTPUT 1

void MutatorFrontendAction_455::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;

        if (MT->getParent()->isClass() && MT->getParent()->isTemplated()) {
            auto methodName = MT->getNameAsString();
            if (overloadedFunctions.find(methodName) == overloadedFunctions.end()) {
                overloadedFunctions.insert(methodName);

                std::string newOverload = "void " + methodName + "(double) { }";
                SourceLocation insertLoc = MT->getEndLoc().getLocWithOffset(1);
                Rewrite.InsertText(insertLoc, "\n/*mut455*/" + newOverload);
            }
        }
    }
}

void MutatorFrontendAction_455::MutatorASTConsumer_455::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}