//source file
#include "../include/insert_recursive_templated_function_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            std::string funcName = FD->getNameAsString();
            std::string templateCode = "\n/*mut3*/\n"
                                       "template<typename T>\n"
                                       "T recursive_" + funcName + "(T n) {\n"
                                       "  if (n <= 1) return 1;\n"
                                       "  return n * recursive_" + funcName + "(n - 1);\n"
                                       "}\n";

            SourceLocation insertLoc = FD->getEndLoc().getLocWithOffset(1);
            Rewrite.InsertText(insertLoc, templateCode, true, true);
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}