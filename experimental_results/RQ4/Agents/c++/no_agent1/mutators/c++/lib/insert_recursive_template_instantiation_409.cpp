//source file
#include "../include/insert_recursive_template_instantiation_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->isThisDeclarationADefinition()) {
            auto templateName = TD->getNameAsString();
            auto sourceRange = TD->getSourceRange();
            auto templateText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            std::string recursiveTemplate = R"(
template<int N>
struct )" + templateName + R"(_Recursive {
    static const int value = N + )" + templateName + R"(_Recursive<N-1>::value;
};

template<>
struct )" + templateName + R"(_Recursive<0> {
    static const int value = 0;
};

// Trigger instantiation
int trigger = )" + templateName + R"(_Recursive<10>::value;
)";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), templateText + recursiveTemplate);
        }
    }
}
  
void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}