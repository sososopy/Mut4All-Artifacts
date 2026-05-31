//source file
#include "../include/modify_template_parameters_in_specialization_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateSpecialization")) {
        if (const auto *TS = FD->getTemplateSpecializationInfo()) {
            if (!Result.Context->getSourceManager().isWrittenInMainFile(
                    FD->getLocation()))
                return;

            if (const auto *ArgsAsWritten = TS->TemplateArgumentsAsWritten) {
                SourceLocation StartLoc = ArgsAsWritten->getLAngleLoc();
                SourceLocation EndLoc = ArgsAsWritten->getRAngleLoc();

                CharSourceRange Range(SourceRange(StartLoc, EndLoc), false);
                std::string NewTemplateArgs = "<char, double>"; // Modify template parameters to introduce inconsistency

                Rewrite.ReplaceText(Range, NewTemplateArgs);
            }
        }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(has(templateSpecializationType())).bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}