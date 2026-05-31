//source file
#include "../include/Add_Remove_Const_Template_Arg_235.h"

// ========================================================================================================
#define MUT235_OUTPUT 1

void MutatorFrontendAction_235::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *param = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
        if (!param || !Result.Context->getSourceManager().isWrittenInMainFile(param->getLocation()))
            return;

        QualType paramType = param->getType();
        clang::TypeLoc typeLoc = param->getTypeSourceInfo()->getTypeLoc();
        if (const clang::TemplateSpecializationTypeLoc &TSTLoc = typeLoc.getAs<clang::TemplateSpecializationTypeLoc>()) {
            for (unsigned i = 0; i < TSTLoc.getNumArgs(); ++i) {
                const clang::TemplateArgumentLoc &argLoc = TSTLoc.getArgLoc(i);
                if (argLoc.getArgument().getKind() == clang::TemplateArgument::Type) {
                    QualType argType = argLoc.getArgument().getAsType();
                    clang::SourceRange argRange = argLoc.getSourceRange();
                    std::string argText = clang::Lexer::getSourceText(
                        clang::CharSourceRange::getTokenRange(argRange),
                        *Result.SourceManager,
                        Result.Context->getLangOpts()
                    ).str();
                    bool addConst = getrandom::getRandomIndex(1) == 0;
                    if (addConst) {
                        if (!argType.isConstQualified()) {
                            std::string newArgText = "const " + argText;
                            Rewrite.ReplaceText(argRange, newArgText);
                        }
                    } else {
                        if (argType.isConstQualified()) {
                            size_t pos = argText.find("const ");
                            if (pos == 0) {
                                std::string newArgText = argText.substr(6);
                                Rewrite.ReplaceText(argRange, newArgText);
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_235::MutatorASTConsumer_235::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::parmVarDecl(hasType(templateSpecializationType())).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}