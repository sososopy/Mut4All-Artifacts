//source file
#include "../include/Modify_Template_Parameter_Pack_Expansion_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *Spec = dyn_cast<ClassTemplateSpecializationDecl>(TD->getTemplatedDecl())) {
            auto SourceRange = Spec->getSourceRange();
            auto OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);

            std::string MutatedText = OriginalText;
            size_t Pos = MutatedText.find("struct bar");
            if (Pos != std::string::npos) {
                MutatedText.insert(Pos + 10, ", size_t... N");
                size_t FuncPos = MutatedText.find("auto func");
                if (FuncPos != std::string::npos) {
                    size_t ParamPos = MutatedText.find(")", FuncPos);
                    if (ParamPos != std::string::npos) {
                        MutatedText.insert(ParamPos, ", int (&...arr)[N]");
                    }
                }
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
        }
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        hasDescendant(cxxRecordDecl(isDerivedFrom(cxxRecordDecl())))
    ).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}