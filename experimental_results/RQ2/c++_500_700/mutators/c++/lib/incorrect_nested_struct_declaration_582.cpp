//source file
#include "../include/incorrect_nested_struct_declaration_582.h"

// ========================================================================================================
#define MUT582_OUTPUT 1

void MutatorFrontendAction_582::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        if (SD->isStruct() && SD->isCompleteDefinition()) {
            auto structName = SD->getNameAsString();
            std::string incorrectNestedDecl = "struct " + structName + ";";
            SourceLocation insertLoc = SD->getLocation().getLocWithOffset(1);
            Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut582*/" + incorrectNestedDecl);
        }
    }
}

void MutatorFrontendAction_582::MutatorASTConsumer_582::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}