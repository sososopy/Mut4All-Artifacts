//source file
#include "../include/Modify_Default_Template_Args_149.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AliasDecl = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasDecl")) {
        if (!AliasDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                AliasDecl->getLocation()))
            return;

        const auto *TypeAlias = AliasDecl->getTemplatedDecl();
        if (!TypeAlias)
            return;

        const auto *TypeExpr = TypeAlias->getTypeSourceInfo()->getType().getTypePtrOrNull();
        if (!TypeExpr)
            return;

        const auto *TemplateParam = AliasDecl->getTemplateParameters()->getParam(0);
        if (!TemplateParam)
            return;

        if (const auto *TemplateTypeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(TemplateParam)) {
            if (!TemplateTypeParam->hasDefaultArgument())
                return;

            const clang::TypeSourceInfo *DefaultArg = TemplateTypeParam->getDefaultArgumentInfo();
            if (!DefaultArg)
                return;

            std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager),
                                                               DefaultArg->getTypeLoc().getSourceRange());
            std::string MutatedText = OriginalText + "*";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(DefaultArg->getTypeLoc().getSourceRange()), MutatedText);
        }
    }
}

void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl(has(typeAliasDecl(hasType(asString("point<3, T2>"))))).bind("AliasDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}