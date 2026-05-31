//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Member_Function_Template_Specialization_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> specializations;
    };
};

//source file
#include "../include/Change_Member_Function_Template_Specialization_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Specialization")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        
        if (!FD->isFunctionTemplateSpecialization() || !FD->getInstantiatedFromMemberFunction())
            return;
        
        const clang::DeclContext* DC = FD->getParent();
        const clang::CXXRecordDecl* RD = clang::dyn_cast<clang::CXXRecordDecl>(DC);
        if (!RD || !RD->isCompleteDefinition())
            return;
        
        SourceLocation ClassEnd = RD->getEndLoc();
        SourceLocation SpecLoc = FD->getBeginLoc();
        if (!Result.SourceManager->isBeforeInTranslationUnit(SpecLoc, ClassEnd))
            return;
        
        auto specializationText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        
        Rewrite.RemoveText(FD->getSourceRange());
        
        specializations.push_back(FD);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDef")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
            return;
        
        if (!RD->isCompleteDefinition())
            return;
        
        for (const clang::FunctionDecl* FD : specializations) {
            if (FD->getParent() != RD)
                continue;
            
            std::string outOfLineDef;
            
            if (const clang::ClassTemplateDecl* CTD = RD->getDescribedClassTemplate()) {
                std::string classTemplateParams;
                const clang::TemplateParameterList* TPL = CTD->getTemplateParameters();
                if (TPL) {
                    classTemplateParams = "template<";
                    for (unsigned i = 0; i < TPL->size(); ++i) {
                        const clang::NamedDecl* ND = TPL->getParam(i);
                        if (i > 0) classTemplateParams += ", ";
                        if (const clang::TemplateTypeParmDecl* TTP = clang::dyn_cast<clang::TemplateTypeParmDecl>(ND)) {
                            classTemplateParams += "class " + TTP->getNameAsString();
                        }
                    }
                    classTemplateParams += ">";
                }
                
                outOfLineDef = classTemplateParams + "\n";
                
                std::string funcSig = FD->getNameAsString();
                
                std::string returnType = FD->getReturnType().getAsString();
                
                outOfLineDef += "template<> " + returnType + " " + RD->getNameAsString() + "<T>::" + funcSig + "() {\n";
                
                const clang::CompoundStmt* Body = clang::dyn_cast<clang::CompoundStmt>(FD->getBody());
                if (Body) {
                    auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), Body->getSourceRange());
                    outOfLineDef += bodyText.substr(1, bodyText.length() - 2);
                }
                outOfLineDef += "\n}\n";
                
                SourceLocation insertLoc = RD->getEndLoc();
                Rewrite.InsertTextAfterToken(insertLoc, "\n" + outOfLineDef);
            }
        }
        
        specializations.clear();
    }
}