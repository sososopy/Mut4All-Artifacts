//source file
#include "../include/Add_Variadic_Using_Declaration_With_Virtual_Inheritance_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_467::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetClass")) {
        // Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;

        // Check if class template has parameter pack with virtual inheritance
        if (auto *Template = CL->getDescribedClassTemplate()) {
            for (const auto *Param : Template->getTemplateParameters()->asArray()) {
                if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                    if (TTP->isParameterPack()) {
                        // Check if base uses pack with virtual inheritance
                        for (const auto &Base : CL->bases()) {
                            if (Base.isVirtual() && 
                                Base.getType()->getAs<clang::PackExpansionType>()) {
                                // Generate using declaration with fold expression
                                std::string packName = TTP->getNameAsString();
                                std::string usingDecl = "using " + packName + "::member...;\n";
                                usingDecl = "/*mut467*/" + usingDecl;
                                // Insert before closing brace
                                SourceLocation loc = CL->getEndLoc();
                                if (loc.isValid()) {
                                    Rewrite.InsertText(loc, usingDecl, true, true);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_467::MutatorASTConsumer_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify class templates with virtual inheritance from parameter pack
    DeclarationMatcher matcher = cxxRecordDecl(
        isTemplateInstantiation(), 
        has(cxxBaseSpecifier(isVirtual()))
    ).bind("TargetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}