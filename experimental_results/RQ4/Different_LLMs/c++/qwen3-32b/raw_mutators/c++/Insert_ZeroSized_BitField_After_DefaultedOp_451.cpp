//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_ZeroSized_BitField_After_DefaultedOp_451
 */ 
class MutatorFrontendAction_451 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(451)

private:
    class MutatorASTConsumer_451 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_451(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_ZeroSized_BitField_After_DefaultedOp_451.h"

// ========================================================================================================
#define MUT451_OUTPUT 1

void MutatorFrontendAction_451::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *classDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classDecl")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(classDecl->getLocation()))
            return;
        for (const auto *field : classDecl->fields()) {
            if (field->getType().isBitFieldType() && field->hasName()) {
                clang::QualType fieldType = field->getType().getUnqualifiedType();
                std::string fieldTypeStr;
                {
                    llvm::raw_string_ostream os(fieldTypeStr);
                    fieldType.print(os);
                }
                std::string newBitField = fieldTypeStr + " : 0;";
                SourceLocation endLoc = field->getSourceRange().getEnd();
                Rewrite.InsertTextAfterToken(endLoc, "\n    " + newBitField);
            }
        }
    }
}
  
void MutatorFrontendAction_451::MutatorASTConsumer_451::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        hasMethod(cxxMethodDecl(
            hasName("operator=="), 
            isDefaulted()
        )),
        has(fieldDecl(isBitField(), hasName(!"")))
    ).bind("classDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}