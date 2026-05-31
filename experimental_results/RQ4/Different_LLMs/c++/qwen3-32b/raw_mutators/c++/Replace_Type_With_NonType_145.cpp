//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/TemplateBase.h"

/**
 * Replace_Type_With_NonType_145
 */ 
class MutatorFrontendAction_145 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(145)

private:
    class MutatorASTConsumer_145 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_145(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Type_With_NonType_145.h"

// ========================================================================================================
#define MUT145_OUTPUT 1

void MutatorFrontendAction_145::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        TemplateParameterList *TPL = CT->getTemplateParameters();
        if (!TPL || TPL->size() == 0)
            return;

        TemplateTypeParmDecl *TTP = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(0));
        if (!TTP)
            return;

        SourceRange paramRange = TTP->getSourceRange();
        std::string paramText = stringutils::rangetoStr(*Result.SourceManager, paramRange);
        if (paramText.find("typename") != std::string::npos || paramText.find("class") != std::string::npos) {
            Rewrite.ReplaceText(paramRange, "int N");
        }

        CXXRecordDecl *Record = CT->getTemplatedDecl();
        if (Record) {
            SourceRange recordRange = Record->getSourceRange();
            std::string recordText = stringutils::rangetoStr(*Result.SourceManager, recordRange);
            size_t pos = 0;
            while ((pos = recordText.find("T", pos)) != std::string::npos) {
                recordText.replace(pos, 1, "N");
                pos += 1;
            }
            Rewrite.ReplaceText(recordRange, recordText);
        }
    }
}
  
void MutatorFrontendAction_145::MutatorASTConsumer_145::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(templateParameterList(hasParam(0, templateTypeParmDecl())))).bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}