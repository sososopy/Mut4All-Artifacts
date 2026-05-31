//source file
#include "../include/modify_function_overloading_with_explicit_object_parameter_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MD->getLocation()))
            return;

        if (MD->isOverloadedOperator()) {
            methods.push_back(MD);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        for (auto *MD : methods) {
            if (FD->getNameAsString() == MD->getNameAsString()) {
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                           FD->getSourceRange());
                std::string mutatedDeclaration = "bool operator==(this const int&, const B&);";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedDeclaration);
                return;
            }
        }
    }
}
  
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher methodMatcher = cxxMethodDecl().bind("Methods");
    DeclarationMatcher funcMatcher = functionDecl().bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}