//header file
#pragma once
#include "Mutator_base.h"

/**
 * PseudoDestructorPackExpansionInsertion_211
 */ 
class MutatorFrontendAction_211 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(211)

private:
    class MutatorASTConsumer_211 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_211(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/PseudoDestructorPackExpansionInsertion_211.h"

// ========================================================================================================
#define MUT211_OUTPUT 1

void MutatorFrontendAction_211::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        if (MD->isConst())
            return;

        const CXXRecordDecl *RD = MD->getParent();
        if (!RD || !RD->getDescribedClassTemplate())
            return;

        const ClassTemplateDecl *CTD = RD->getDescribedClassTemplate();
        if (!CTD->isVariadic())
            return;

        if (!MD->hasBody())
            return;

        const Stmt *Body = MD->getBody();
        llvm::SmallVector<std::pair<SourceRange, std::string>, 4> exprReplacements;

        auto visitor = [&](const Stmt *S) {
            if (const auto *ME = dyn_cast<MemberExpr>(S)) {
                if (!ME->isArrow()) {
                    const Expr *Base = ME->getBase();
                    if (const auto *UO = dyn_cast<UnaryOperator>(Base)) {
                        if (UO->getOpcode() == UO_Deref) {
                            const Expr *Operand = UO->getSubExpr();
                            if (const auto *DRE = dyn_cast<DeclRefExpr>(Operand)) {
                                std::string varName = DRE->getNameAsString();
                                const TemplateParameterList *TPL = CTD->getTemplateParameters();
                                std::string paramName;
                                for (unsigned i = 0; i < TPL->size(); ++i) {
                                    const TemplateParameter *Param = TPL->getParam(i);
                                    if (const auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
                                        if (TTP->isParameterPack()) {
                                            paramName = TTP->getNameAsString();
                                            break;
                                        }
                                    }
                                }
                                if (paramName.empty())
                                    return;
                                std::string destructorCall = "(*" + varName + ").~" + paramName + "...";
                                if (getrandom::getRandomIndex(2) == 0) {
                                    destructorCall += "[]();";
                                } else {
                                    destructorCall += ";";
                                }
                                exprReplacements.push_back(std::make_pair(ME->getSourceRange(), destructorCall));
                            }
                        }
                    }
                }
                for (const Stmt *Child : S->children()) {
                    if (Child)
                        visitor(Child);
                }
            }
        };
        visitor(Body);

        if (!exprReplacements.empty()) {
            int index = getrandom::getRandomIndex(exprReplacements.size());
            const auto &entry = exprReplacements[index];
            Rewrite.ReplaceText(entry.first, entry.second);
        }
    }
}

void MutatorFrontendAction_211::MutatorASTConsumer_211::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        ofClass(classTemplateDecl(isVariadic())),
        unless(isConst()),
        hasBody(stmt(anything()))
    ).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}