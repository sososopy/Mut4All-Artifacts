//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Array_StructuredBinding_476
 */ 
class MutatorFrontendAction_476 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(476)

private:
    class MutatorASTConsumer_476 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_476(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Array_StructuredBinding_476.h"

// ========================================================================================================
#define MUT476_OUTPUT 1

void MutatorFrontendAction_476::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *arrayDecl = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        if (!arrayDecl || !Result.Context->getSourceManager().isWrittenInMainFile(arrayDecl->getLocation()))
            return;

        if (auto *bindingDecl = Result.Nodes.getNodeAs<clang::StructuredBindingDecl>("bindingDecl")) {
            if (!bindingDecl || !Result.Context->getSourceManager().isWrittenInMainFile(bindingDecl->getLocation()))
                return;

            auto arrayType = arrayDecl->getType()->getAs<clang::ConstantArrayType>();
            if (!arrayType)
                return;
            auto size = arrayType->getSize().getZExtValue();

            int d1 = 2;
            int d2 = size / d1;

            auto typeLoc = arrayDecl->getTypeSourceInfo()->getTypeLoc();
            auto arrayTypeLoc = typeLoc.getAs<clang::ConstantArrayTypeLoc>();
            if (!arrayTypeLoc)
                return;

            SourceLocation lbracketLoc = arrayTypeLoc.getLBracketLoc();
            SourceLocation rbracketLoc = arrayTypeLoc.getRBracketLoc();
            if (lbracketLoc.isInvalid() || rbracketLoc.isInvalid())
                return;

            std::string newDimensions = "[" + std::to_string(d1) + "][" + std::to_string(d2) + "]";
            Rewrite.ReplaceText(SourceRange(lbracketLoc, rbracketLoc), newDimensions);

            auto bindings = bindingDecl->getBindings();
            if (bindings.size() != 1)
                return;

            std::string newVarNames;
            for (int i = 0; i < d1; ++i) {
                char c = 'a' + i;
                newVarNames += std::string(1, c);
                if (i < d1 - 1)
                    newVarNames += ", ";
            }

            auto SM = Result.Context->getSourceManager();
            auto bindingText = stringutils::rangetoStr(*SM, bindingDecl->getSourceRange());

            size_t lbracketPos = bindingText.find('[');
            size_t rbracketPos = bindingText.find(']');
            if (lbracketPos == std::string::npos || rbracketPos == std::string::npos)
                return;

            std::string newBindingText = bindingText.substr(0, lbracketPos + 1) + newVarNames + bindingText.substr(rbracketPos);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bindingDecl->getSourceRange()), newBindingText);
        }
    }
}
  
void MutatorFrontendAction_476::MutatorASTConsumer_476::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        varDecl(isConstexpr(), hasType(arrayType(constantArrayType())), hasInitializer(implicitValueInitExpr()))
            .bind("arrayDecl") &&
        hasDescendant(
            structuredBindingDecl(hasInitializer(declRefExpr(to(varDecl(equalsBoundNode("arrayDecl"))))))
                .bind("bindingDecl")
        );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}