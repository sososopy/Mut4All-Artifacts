//source file
#include "../include/remove_in_class_initializer_from_union_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("unionDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        for (auto *field : UD->fields()) {
            if (field->hasInClassInitializer()) {
                auto fieldRange = field->getSourceRange();
                auto initRange = field->getInClassInitializer()->getSourceRange();
                std::string fieldText = Lexer::getSourceText(CharSourceRange::getTokenRange(fieldRange), 
                                                             *Result.SourceManager, 
                                                             Result.Context->getLangOpts()).str();
                std::string initText = Lexer::getSourceText(CharSourceRange::getTokenRange(initRange), 
                                                            *Result.SourceManager, 
                                                            Result.Context->getLangOpts()).str();

                size_t initPos = fieldText.find(initText);
                if (initPos != std::string::npos) {
                    fieldText.erase(initPos);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(fieldRange), fieldText);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isUnion()).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}