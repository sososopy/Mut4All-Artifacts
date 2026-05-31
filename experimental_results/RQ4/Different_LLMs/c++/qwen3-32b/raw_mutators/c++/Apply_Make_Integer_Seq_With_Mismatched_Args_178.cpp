//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Make_Integer_Seq_With_Mismatched_Args_178
 */ 
class MutatorFrontendAction_178 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(178)
private:
    class MutatorASTConsumer_178 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_178(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Apply_Make_Integer_Seq_With_Mismatched_Args_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *field = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!field || !Result.Context->getSourceManager().isWrittenInMainFile(field->getLocation()))
            return;
        
        QualType fieldType = field->getType();
        if (auto *TST = dyn_cast<clang::TemplateSpecializationType>(fieldType)) {
            clang::TemplateDecl *TD = TST->getTemplate();
            if (auto *CTD = dyn_cast<clang::ClassTemplateDecl>(TD)) {
                auto *TemplatedDecl = CTD->getTemplatedDecl();
                if (TemplatedDecl && !TemplatedDecl->isThisDeclarationADefinition()) {
                    std::string tName = CTD->getNameAsString();
                    std::string newType = "__make_integer_seq<" + tName + ", float, 10>";
                    clang::SourceRange typeRange = field->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                    if (typeRange.isValid()) {
                        Rewrite.ReplaceText(typeRange, newType);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(parent(cxxRecordDecl())).bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}