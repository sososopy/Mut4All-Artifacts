//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constexpr_CTAD_Class_73
 */ 
class MutatorFrontendAction_AddConstexprCTADClass_73 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(AddConstexprCTADClass_73)
private:
    class MutatorASTConsumer_AddConstexprCTADClass_73 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_AddConstexprCTADClass_73(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Constexpr_CTAD_Class_73.h"

// ========================================================================================================
#define MUTAddConstexprCTADClass_73_OUTPUT 1

void MutatorFrontendAction_AddConstexprCTADClass_73::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *StructDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl")) {
        if (!StructDecl || !Result.Context->getSourceManager().isWrittenInMainFile(StructDecl->getLocation()))
            return;
        
        std::string StructName = StructDecl->getNameAsString();
        std::string NewStructName = StructName + "_Copy";
        
        std::string TemplateParams;
        if (auto *TPL = StructDecl->getTemplateParameterList()) {
            for (unsigned i = 0; i < TPL->size(); ++i) {
                if (i > 0) TemplateParams += ", ";
                TemplateParams += TPL->getParam(i)->getNameAsString();
            }
            TemplateParams = "template<" + TemplateParams + ">";
        }
        
        std::string ConstructorCode;
        for (const auto *Ctor : StructDecl->ctors()) {
            if (Ctor->isConstexpr()) {
                std::string CtorParams;
                for (unsigned i = 0; i < Ctor->getNumParams(); ++i) {
                    if (i > 0) CtorParams += ", ";
                    CtorParams += Ctor->parameters()[i]->getType().getAsString();
                    CtorParams += " ";
                    CtorParams += Ctor->parameters()[i]->getNameAsString();
                }
                ConstructorCode = "    consteval " + NewStructName + "(" + CtorParams + ") {}";
                break;
            }
        }
        
        std::string NewStructCode = TemplateParams + "\nstruct " + NewStructName + " {\n" + ConstructorCode + "\n};\n";
        SourceLocation InsertLoc = StructDecl->getEndLoc();
        Rewrite.InsertTextAfterToken(InsertLoc, NewStructCode);
    }
    
    if (auto *VarDecl = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VarDecl || !Result.Context->getSourceManager().isWrittenInMainFile(VarDecl->getLocation()))
            return;
        
        std::string VarName = VarDecl->getNameAsString();
        std::string NewVarName = VarName + "_copy";
        std::string VarType = VarDecl->getType().getAsString();
        std::string NewVarType = VarType;
        auto *DC = VarDecl->getDeclContext();
        if (auto *ND = dyn_cast<clang::NamedDecl>(DC)) {
            size_t Pos = NewVarType.find(ND->getNameAsString());
            if (Pos != std::string::npos) {
                NewVarType.replace(Pos, ND->getNameAsString().length(), VarName + "_Copy");
            }
        }
        
        std::string Initializer;
        if (auto *Init = VarDecl->getInit()) {
            Initializer = stringutils::rangetoStr(*Result.SourceManager, Init->getSourceRange());
        }
        
        std::string NewVarCode = "constexpr " + NewVarType + " " + NewVarName + " = " + Initializer + ";\n";
        SourceLocation InsertLoc = VarDecl->getEndLoc();
        Rewrite.InsertTextAfterToken(InsertLoc, NewVarCode);
    }
}

void MutatorFrontendAction_AddConstexprCTADClass_73::MutatorASTConsumer_AddConstexprCTADClass_73::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher structMatcher = cxxRecordDecl(
        isStruct(),
        isClassTemplate(),
        hasDescendant(cxxConstructorDecl(isConstexpr()))
    ).bind("structDecl");
    DeclarationMatcher varMatcher = varDecl(
        isConstexpr(),
        hasType(templateSpecializationType(hasDeclaration(cxxRecordDecl()))),
        hasInitializer(ignoringImplicit(lambdaExpr()))
    ).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}