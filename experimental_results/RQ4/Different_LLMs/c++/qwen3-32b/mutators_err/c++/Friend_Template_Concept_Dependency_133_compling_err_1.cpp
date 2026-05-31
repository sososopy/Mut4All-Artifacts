//header file
#pragma once
#include "Mutator_base.h"

/**
 * Friend_Template_Concept_Dependency_133
 */ 
class MutatorFrontendAction_133 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(133)
private:
    class MutatorASTConsumer_133 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_133(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Friend_Template_Concept_Dependency_133.h"

// ========================================================================================================
#define MUT133_OUTPUT 1

void MutatorFrontendAction_133::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FriendDecl>("friendDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        clang::QualType FriendType = FD->getFriendType();
        if (auto *Tmpl = FriendType->getAsTemplateDecl<clang::ClassTemplateDecl>()) {
            clang::Decl *OuterParent = FD->getParent();
            if (auto *OuterClass = clang::dyn_cast<clang::CXXRecordDecl>(OuterParent)) {
                if (auto *OuterTmpl = OuterClass->getTemplateInstantiationPattern()) {
                    clang::TemplateParameterList *OuterTmplParams = OuterTmpl->getTemplateParameters();
                    if (OuterTmplParams && OuterTmplParams->size() >= 1) {
                        clang::TemplateTypeParmDecl *OuterTParam = dyn_cast<clang::TemplateTypeParmDecl>(OuterTmplParams->getParam(0));
                        if (OuterTParam && OuterTParam->getConcept()) {
                            clang::TemplateParameterList *FriendTmplParams = Tmpl->getTemplateParameters();
                            if (FriendTmplParams && FriendTmplParams->size() >= 1) {
                                clang::TemplateTypeParmDecl *FriendTParam = dyn_cast<clang::TemplateTypeParmDecl>(FriendTmplParams->getParam(0));
                                if (FriendTParam && FriendTParam->getConcept()) {
                                    clang::ConceptReference *FriendConcept = FriendTParam->getConcept();
                                    clang::ConceptReference *OuterConcept = OuterTParam->getConcept();
                                    if (!FriendConcept || !OuterConcept)
                                        return;

                                    std::string outerConceptName = OuterConcept->getName().str();
                                    std::string tName = OuterTParam->getNameAsString();
                                    std::string newConstraint = outerConceptName + "<decltype(" + tName + "::nested)>";
                                    // Now, replace the friend's concept with newConstraint
                                    clang::SourceLocation loc = FriendConcept->getLocation();
                                    std::string oldConstraint = FriendConcept->getName().str();
                                    Rewrite.ReplaceText(loc, oldConstraint.size(), newConstraint);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_133::MutatorASTConsumer_133::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = friendDecl(hasFriendType(classTemplateDecl())).bind("friendDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}