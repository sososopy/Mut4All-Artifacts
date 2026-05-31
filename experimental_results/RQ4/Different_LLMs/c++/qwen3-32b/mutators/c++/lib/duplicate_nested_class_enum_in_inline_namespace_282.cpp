//source file
#include "../include/Duplicate_Nested_Class_Enum_In_Inline_Namespace_282.h"

// ========================================================================================================
#define MUT282_OUTPUT 1

void MutatorFrontendAction_282::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *decl = Result.Nodes.getNodeAs<clang::Decl>("decl")) {
        if (!decl || !Result.Context->getSourceManager().isWrittenInMainFile(decl->getLocation()))
            return;

        if (auto *recordDecl = dyn_cast<clang::CXXRecordDecl>(decl)) {
            if (!recordDecl->isCompleteDefinition())
                return;

            SourceRange braceRange = recordDecl->getBraceRange();
            if (braceRange.isInvalid())
                return;

            std::string name = recordDecl->getNameAsString();
            if (name.empty())
                return;

            std::string insertion = "struct " + name + " { };\n";
            SourceLocation endBraceLoc = braceRange.getEnd();
            Rewrite.InsertText(endBraceLoc, insertion);
        } else if (auto *namespaceDecl = dyn_cast<clang::NamespaceDecl>(decl)) {
            if (!namespaceDecl->isInline())
                return;

            std::string name = namespaceDecl->getNameAsString();
            if (name.empty())
                return;

            std::string insertion = "struct " + name + " { };\n";
            SourceLocation endLoc = namespaceDecl->getSourceRange().getEnd();
            Rewrite.InsertText(endLoc, insertion);
        }
    }
}
  
void MutatorFrontendAction_282::MutatorASTConsumer_282::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = anyOf(cxxRecordDecl().bind("decl"), namespaceDecl(isInline()).bind("decl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}