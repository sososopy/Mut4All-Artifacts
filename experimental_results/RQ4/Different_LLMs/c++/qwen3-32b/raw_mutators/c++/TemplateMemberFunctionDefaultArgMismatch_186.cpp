//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateMemberFunctionDefaultArgMismatch_186
 */ 
class MutatorFrontendAction_186 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(186)

private:
    class MutatorASTConsumer_186 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_186(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXMethodDecl *> primaryMethods;
        std::vector<const clang::CXXMethodDecl *> specializationMethods;
    };
};

//source file
#include "../include/Mutator_TemplateMemberFunctionDefaultArgMismatch_186.h"

// ========================================================================================================
#define MUT186_OUTPUT 1

void MutatorFrontendAction_186::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *primaryMethod = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("primaryMethod")) {
        if (!primaryMethod || !Result.Context->getSourceManager().isWrittenInMainFile(
                       primaryMethod->getLocation()))
            return;

        if (!primaryMethod->isTemplateInstantiation() || !primaryMethod->getParent()->isTemplateInstantiation()) {
            return;
        }

        primaryMethods.push_back(primaryMethod);

        ASTContext &Context = *Result.Context;
        auto *TUDecl = Context.getTranslationUnitDecl();
        for (auto *decl : TUDecl->decls()) {
            if (auto *candidateClass = dyn_cast<clang::CXXRecordDecl>(decl)) {
                if (candidateClass->isSpecialization()) {
                    auto *primaryClass = primaryMethod->getParent();
                    if (candidateClass->getTemplateSpecializationArgs() && 
                        candidateClass->getTemplateSpecializationArgs()->getTemplate() == 
                        primaryClass->getTypeForDecl()) {
                        for (auto *method : candidateClass->methods()) {
                            if (method->getNameAsString() == primaryMethod->getNameAsString() &&
                                !method->hasDefaultArgs()) {
                                specializationMethods.push_back(method);
                                for (auto *param : primaryMethod->parameters()) {
                                    if (param->hasDefaultArg()) {
                                        SourceRange defaultArgRange = param->getDefaultArg()->getSourceRange();
                                        Rewrite.RemoveText(defaultArgRange);
                                    }
                                }
                                if (!method->parameters().empty()) {
                                    auto *firstParam = method->parameters()[0];
                                    QualType paramType = firstParam->getType();
                                    std::string typeName = paramType.getAsString();
                                    std::string defaultArg = typeName + "()";
                                    SourceLocation endLoc = firstParam->getSourceRange().getEnd();
                                    Rewrite.InsertTextAfter(endLoc, " = " + defaultArg);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (auto *specializationMethod = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("specializationMethod")) {
        if (!specializationMethod || !Result.Context->getSourceManager().isWrittenInMainFile(
                           specializationMethod->getLocation()))
            return;

        if (!specializationMethod->getParent()->isSpecialization() || 
            specializationMethod->hasDefaultArgs()) {
            return;
        }

        specializationMethods.push_back(specializationMethod);
    }
}

void MutatorFrontendAction_186::MutatorASTConsumer_186::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher primaryMatcher = cxxMethodDecl(
        isTemplate(), 
        ofClass(cxxRecordDecl(isTemplate())), 
        hasDefaultArgs()
    ).bind("primaryMethod");

    DeclarationMatcher specializationMatcher = cxxMethodDecl(
        ofClass(cxxRecordDecl(isSpecialization())), 
        unless(hasDefaultArgs())
    ).bind("specializationMethod");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(primaryMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.matchAST(Context);
}