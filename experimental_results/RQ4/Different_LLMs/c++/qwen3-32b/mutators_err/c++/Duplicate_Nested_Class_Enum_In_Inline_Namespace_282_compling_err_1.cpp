//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Nested_Class_Enum_In_Inline_Namespace_282
 */ 
class MutatorFrontendAction_282 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(282)

private:
    class MutatorASTConsumer_282 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_282(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

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
    DeclarationMatcher matcher = (cxxRecordDecl().bind("decl")) || (namespaceDecl(isInline()).bind("decl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}