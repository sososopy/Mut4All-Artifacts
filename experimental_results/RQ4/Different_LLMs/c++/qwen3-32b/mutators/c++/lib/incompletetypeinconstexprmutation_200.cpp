//source file
#include "../include/IncompleteTypeInConstexprMutation_200.h"

// ========================================================================================================
#define MUT200_OUTPUT 1

void MutatorFrontendAction_200::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *varDecl = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVar")) {
        if (!varDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                       varDecl->getLocation()))
            return;

        QualType varType = varDecl->getType();
        if (varType->getTypeClass() != clang::Type::Record)
            return;

        const clang::RecordType *recordType = cast<clang::RecordType>(varType.getTypePtr());
        const clang::CXXRecordDecl *recordDecl = dyn_cast<clang::CXXRecordDecl>(recordType->getDecl());

        if (!recordDecl || !recordDecl->isCompleteDefinition())
            return;

        if (!recordDecl->isStruct() && !recordDecl->isClass())
            return;

        if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;

        std::string typeName = recordDecl->getNameAsString();
        if (processedTypes.find(typeName) != processedTypes.end())
            return;

        processedTypes.insert(typeName);

        SourceRange recordRange = recordDecl->getSourceRange();
        std::string replacement;
        if (recordDecl->isStruct()) {
            replacement = "struct " + typeName + ";";
        } else {
            replacement = "class " + typeName + ";";
        }

        Rewrite.ReplaceText(recordRange, replacement);
    }
}
  
void MutatorFrontendAction_200::MutatorASTConsumer_200::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(isConstexpr(), hasType(recordType())).bind("constexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}