//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Param_With_Fixed_Type_162
 */ 
class MutatorFrontendAction_162 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(162)

private:
    class MutatorASTConsumer_162 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_162(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Param_With_Fixed_Type_162.h"

// ========================================================================================================
#define MUT162_OUTPUT 1

void MutatorFrontendAction_162::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *partialSpec = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("partialSpec")) {
        if (!partialSpec || !Result.Context->getSourceManager().isWrittenInMainFile(partialSpec->getLocation()))
            return;

        CXXRecordDecl *recordDecl = partialSpec->getAsRecordDecl();
        if (!recordDecl || !recordDecl->isCompleteDefinition())
            return;

        for (auto *member : recordDecl->decls()) {
            if (auto *field = dyn_cast<FieldDecl>(member)) {
                QualType fieldType = field->getType();
                if (auto *ttpt = fieldType->getAs<TemplateTypeParmType>()) {
                    TemplateTypeParmDecl *ttdecl = ttpt->getDecl();
                    for (auto *param : partialSpec->getTemplateParameters()) {
                        if (auto *ttdecl2 = dyn_cast<TemplateTypeParmDecl>(param)) {
                            if (ttdecl2 == ttdecl) {
                                SourceRange range = field->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                                Rewrite.ReplaceText(range, "int");
                                break;
                            }
                        }
                    }
                }
            } else if (auto *method = dyn_cast<CXXMethodDecl>(member)) {
                QualType returnType = method->getReturnType();
                if (auto *ttpt = returnType->getAs<TemplateTypeParmType>()) {
                    TemplateTypeParmDecl *ttdecl = ttpt->getDecl();
                    for (auto *param : partialSpec->getTemplateParameters()) {
                        if (auto *ttdecl2 = dyn_cast<TemplateTypeParmDecl>(param)) {
                            if (ttdecl2 == ttdecl) {
                                SourceRange range = method->getReturnTypeSourceRange();
                                Rewrite.ReplaceText(range, "int");
                                break;
                            }
                        }
                    }
                }

                for (auto *param : method->parameters()) {
                    QualType paramType = param->getType();
                    if (auto *ttpt = paramType->getAs<TemplateTypeParmType>()) {
                        TemplateTypeParmDecl *ttdecl = ttpt->getDecl();
                        for (auto *param2 : partialSpec->getTemplateParameters()) {
                            if (auto *ttdecl2 = dyn_cast<TemplateTypeParmDecl>(param2)) {
                                if (ttdecl2 == ttdecl) {
                                    SourceRange range = param->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                                    Rewrite.ReplaceText(range, "int");
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_162::MutatorASTConsumer_162::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("partialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}