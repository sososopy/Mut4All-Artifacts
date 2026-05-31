//source file
#include "../include/Mutator_555.h"

// ========================================================================================================
#define MUT555_OUTPUT 1

void MutatorFrontendAction_555::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UTD = Result.Nodes.getNodeAs<TypeAliasDecl>("AliasTemplate")) {
        if (!UTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UTD->getLocation()))
            return;

        if (auto *TT = dyn_cast<TypeAliasTemplateDecl>(UTD->getDescribedAliasTemplate())) {
            auto *TAD = TT->getTemplatedDecl();
            auto *TD = TAD->getUnderlyingType().getTypePtr();

            if (auto *TST = dyn_cast<TemplateSpecializationType>(TD)) {
                std::string replacementText = TAD->getNameAsString() + "<";

                for (unsigned i = 0; i < TST->getNumArgs(); ++i) {
                    if (i != 0) {
                        replacementText += ", ";
                    }
                    replacementText += TST->getArg(i).getAsType().getAsString();
                }
                replacementText += ">";

                auto sourceRange = UTD->getSourceRange();
                Rewrite.ReplaceText(sourceRange, replacementText);
            }
        }
    }
}

void MutatorFrontendAction_555::MutatorASTConsumer_555::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasDescendant(typeAliasTemplateDecl())).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}