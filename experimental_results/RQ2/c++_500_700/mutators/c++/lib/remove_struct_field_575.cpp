//source file
#include "../include/remove_struct_field_575.h"

// ========================================================================================================
#define MUT575_OUTPUT 1

void MutatorFrontendAction_575::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;
        
        if (!SD->isStruct() || !SD->isCompleteDefinition())
            return;

        for (auto *field : SD->fields()) {
            if (field->getIdentifier() && !field->isReferenced()) {
                fieldsToRemove.push_back(field);
                break;
            }
        }

        for (auto *field : fieldsToRemove) {
            Rewrite.RemoveText(field->getSourceRange());
        }
    }
}

void MutatorFrontendAction_575::MutatorASTConsumer_575::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}