//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Invalid_Nested_Type_332
 */ 
class MutatorFrontendAction_332 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(332)
private:
    class MutatorASTConsumer_332 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_332(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Invalid_Nested_Type_332.h"

// ========================================================================================================
#define MUT332_OUTPUT 1

void MutatorFrontendAction_332::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("typeTraitMember")) {
        if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(ME->getBeginLoc()))
            return;
        
        auto *Base = ME->getBase()->IgnoreImplicit();
        if (auto *DRE = dyn_cast<clang::DeclRefExpr>(Base)) {
            auto *TST = DRE->getType()->getAs<clang::TemplateSpecializationType>();
            if (!TST)
                return;
            
            SourceRange declRefRange = DRE->getSourceRange();
            auto declRefText = stringutils::rangetoStr(*Result.SourceManager, declRefRange);
            
            size_t lastColon = declRefText.rfind("::");
            if (lastColon == std::string::npos)
                return;
            
            std::string templateName = declRefText.substr(lastColon + 2);
            size_t lAnglePos = templateName.find('<');
            if (lAnglePos != std::string::npos)
                templateName = templateName.substr(0, lAnglePos);
            
            std::string modifiedTemplateName = "__" + templateName;
            std::string modifiedDeclRefText = declRefText.substr(0, lastColon + 2) + modifiedTemplateName + declRefText.substr(lastColon + 2 + templateName.length());
            
            Rewrite.ReplaceText(declRefRange, modifiedDeclRefText);
            
            SourceRange memberRange = ME->getSourceRange();
            auto memberText = stringutils::rangetoStr(*Result.SourceManager, memberRange);
            if (memberText == "::type") {
                Rewrite.ReplaceText(memberRange, "::invalid_type");
            }
        }
    }
}
  
void MutatorFrontendAction_332::MutatorASTConsumer_332::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = memberExpr(
        hasMemberName("type"),
        hasObjectExpression(
            declRefExpr(
                to(
                    templateSpecializationType(
                        templateName(qualType(qualType(hasCanonicalType(qualType(decltypeType(declaresType(recordType(hasName("std::*")))))))))
                    )
                )
            )
        )
    ).bind("typeTraitMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}