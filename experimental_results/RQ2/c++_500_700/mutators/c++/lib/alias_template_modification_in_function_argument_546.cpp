//source file
#include "../include/alias_template_modification_in_function_argument_546.h"

// ========================================================================================================
#define MUT546_OUTPUT 1

void MutatorFrontendAction_546::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getNumParams() == 1) {
            auto paramType = FD->getParamDecl(0)->getType().getAsString();
            std::string aliasTemplate = "template<typename T> using TypeWrapper = std::tuple<T>;\n";
            std::string newFunctionDecl = "void " + FD->getNameAsString() + "(TypeWrapper<" + paramType + "> obj)";
            
            auto functionRange = FD->getSourceRange();
            std::string functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
            
            std::string newFunctionBody = "{\n// Some operations involving std::get<0>(obj)\n" + functionBody + "\n}";
            std::string newFunction = aliasTemplate + newFunctionDecl + newFunctionBody;
            
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionRange), newFunction);
        }
    }
}

void MutatorFrontendAction_546::MutatorASTConsumer_546::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0, parmVarDecl())).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}