//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Param_Type_Mismatch_In_Defaulted_Operator_326
 */ 
class MutatorFrontendAction_326 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(326)

private:
    class MutatorASTConsumer_326 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_326(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Param_Type_Mismatch_In_Defaulted_Operator_326.h"

// ========================================================================================================
#define MUT326_OUTPUT 1

void MutatorFrontendAction_326::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!method || !Result.Context->getSourceManager().isWrittenInMainFile(method->getLocation()))
            return;

        if (!method->isDefaulted() || method->getNameAsString() != "==")
            return;

        if (!method->getDescribedFunctionTemplate())
            return;

        const auto *params = method->parameters();
        if (params->size() < 2)
            return;

        const auto *param1 = params->operator[](0);
        const auto *param2 = params->operator[](1);

        const auto *classDecl = method->getParent();
        if (!classDecl || !classDecl->isTemplateInstantiation())
            return;

        QualType param1Type = param1->getType();
        if (auto tspec = param1Type->getAs<clang::TemplateSpecializationType>()) {
            clang::TemplateName name = tspec->getTemplateName();
            if (auto *tdecl = name.getAsTemplateDecl()) {
                clang::TemplateArgumentListInfo *args = tspec->getTemplateArgs();
                if (args->size() >= 1) {
                    clang::TemplateArgument arg = args->get(0);
                    if (arg.getKind() == clang::TemplateArgument::Type) {
                        QualType tType = arg.getAsType();
                        std::vector<QualType> memberTypes;
                        for (const auto *field : classDecl->fields()) {
                            QualType fieldType = field->getType();
                            if (fieldType != tType) {
                                memberTypes.push_back(fieldType);
                            }
                        }
                        if (memberTypes.empty())
                            return;

                        QualType alternateType = memberTypes[0];

                        if (auto tspec2 = param2->getType()->getAs<clang::TemplateSpecializationType>()) {
                            clang::TemplateArgumentListInfo *args2 = tspec2->getTemplateArgs();
                            if (args2->size() >= 1) {
                                clang::TemplateArgument arg2 = args2->get(0);
                                if (arg2.getKind() == clang::TemplateArgument::Type) {
                                    clang::SourceLocation loc = param2->getTypeSourceInfo()->getTypeLoc().getSourceRange().getBegin();
                                    clang::SourceLocation endLoc = param2->getTypeSourceInfo()->getTypeLoc().getSourceRange().getEnd();
                                    std::string replacement = alternateType.getAsString();
                                    Rewrite.ReplaceText(clang::SourceRange(loc, endLoc), replacement);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_326::MutatorASTConsumer_326::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isDefaulted(), name("operator=="), isTemplated()).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}