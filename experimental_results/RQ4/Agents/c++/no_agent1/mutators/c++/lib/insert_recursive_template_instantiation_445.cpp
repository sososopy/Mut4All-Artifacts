//source file
#include "../include/insert_recursive_template_instantiation_445.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody() && FD->isTemplateInstantiation()) {
            auto funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            std::string recursiveTemplate = "template<int N>\nstruct RecursiveTemplate {\n"
                                            "    static const int value = N + RecursiveTemplate<N-1>::value;\n"
                                            "};\n"
                                            "template<>\nstruct RecursiveTemplate<0> {\n"
                                            "    static const int value = 0;\n"
                                            "};\n";
            funcDecl.insert(0, "/*mut445*/\n" + recursiveTemplate);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDecl);
        }
    }
}

void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}