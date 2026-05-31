//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Alias_Template_With_Explicit_Instantiation_In_Deduction_Guide_Context_139
 */ 
class MutatorFrontendAction_139 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(139)

private:
    class MutatorASTConsumer_139 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_139(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::pair<std::string, std::string>> aliasMap;
    };
};

//source file
#include "../include/Mutator_Replace_Alias_Template_With_Explicit_Instantiation_In_Deduction_Guide_1.h"

// ========================================================================================================
#define MUT139_OUTPUT 1

void MutatorFrontendAction_139::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
        if (!AD || !Result.Context->getSourceManager().isWrittenInMainFile(AD->getLocation()))
            return;
        
        auto *TAD = AD->getTemplatedDecl();
        if (!TAD) return;
        
        auto *UnderlyingType = TAD->getUnderlyingType().getTypePtr();
        if (!UnderlyingType) return;
        
        auto *TST = UnderlyingType->getAs<clang::TemplateSpecializationType>();
        if (!TST) return;
        
        auto *TemplateDecl = TST->getTemplateName().getAsTemplateDecl();
        if (!TemplateDecl) return;
        
        std::string TemplateName = TemplateDecl->getNameAsString();
        std::string ArgsText;
        llvm::raw_string_ostream OS(ArgsText);
        PrintingPolicy PP(Result.Context->getLangOpts());
        PP.PrintCanonicalTypes = true;
        
        for (unsigned I = 0; I < TST->getNumArgs(); ++I) {
            if (I > 0) OS << ", ";
            TST->getArg(I).print(PP, OS, true);
        }
        OS.flush();
        
        aliasMap[AD->getNameAsString()] = std::make_pair(TemplateName, ArgsText);
    }
    else if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("AliasRef")) {
        if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(DRE->getLocation()))
            return;
        
        auto *Decl = DRE->getDecl();
        if (!Decl) return;
        
        std::string AliasName = Decl->getNameAsString();
        auto It = aliasMap.find(AliasName);
        if (It == aliasMap.end()) return;
        
        auto *Parent = Result.Nodes.getNodeAs<clang::Decl>("DecltypeParent");
        if (!Parent) return;
        
        auto *SubExpr = Parent->getUnderlyingExpr();
        if (!SubExpr) return;
        
        auto *CE = llvm::dyn_cast<clang::CXXConstructExpr>(SubExpr);
        if (!CE) return;
        
        if (!CE->isListInitialization() && !CE->isStdInitListInitialization()) return;
        
        std::string Replacement = It->second.first;
        if (!It->second.second.empty()) {
            Replacement += "<" + It->second.second + ">";
        }
        
        SourceRange Range = DRE->getSourceRange();
        if (Range.isValid()) {
            Rewrite.ReplaceText(Range, Replacement);
        }
    }
}
  
void MutatorFrontendAction_139::MutatorASTConsumer_139::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher aliasMatcher = typeAliasTemplateDecl().bind("AliasTemplate");
    StatementMatcher usageMatcher = declRefExpr(
        hasParent(
            cxxConstructExpr(
                hasParent(
                    decl().bind("DecltypeParent")
                )
            )
        ),
        to(typeAliasTemplateDecl())
    ).bind("AliasRef");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.addMatcher(usageMatcher, &callback);
    matchFinder.matchAST(Context);
}